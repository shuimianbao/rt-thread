#include  <rtthread.h>
#include <string.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "gprmc.h"

nmeaPROPERTY * nmea_property(void) 
{
     static nmeaPROPERTY prop =
     {
		0, 0, NMEA_DEF_PARSEBUFF
	 };
     return &prop;
} 

void nmea_trace_buff(const char *buff, int buff_size) 
{
    nmeaTraceFunc func = nmea_property()->trace_func;     
		if(func && buff_size)
         (*func)(buff, buff_size);
} 



/** 
  * \brief Convert string to number 
*/ 
int nmea_atoi(const char *str, int str_sz, int radix) 
{
	char *tmp_ptr;
	char buff[NMEA_CONVSTR_BUF];
	int res = 0;
	
	if(str_sz < NMEA_CONVSTR_BUF)
  {
         memcpy(&buff[0], str, str_sz);
         buff[str_sz] = '\0';
         res = strtol(&buff[0], &tmp_ptr, radix);
  }     
	return res; 
} 
/** 
  * \brief Convert string to fraction number 
*/ 
double nmea_atof(const char *str, int str_sz)
{
	char *tmp_ptr;     
	char buff[NMEA_CONVSTR_BUF];
	double res = 0;
	
  if(str_sz < NMEA_CONVSTR_BUF)
  {
         memcpy(&buff[0], str, str_sz);
         buff[str_sz] = '\0';
         res = strtod(&buff[0], &tmp_ptr);
     
	}
	return res; 
} 

/** 
  * \brief Analyse string (specificate for NMEA sentences) 
*/ 
int nmea_scanf(const char *buff, int buff_sz, const char *format, ...)
{
	const char *beg_tok;
	const char *end_buf = buff + buff_sz;
	va_list arg_ptr;
	int tok_type = NMEA_TOKS_COMPARE;
	int width = 0;
	const char *beg_fmt = 0;
	int snum = 0, unum = 0;
	int tok_count = 0;
	void *parg_target;

	va_start(arg_ptr, format);
	for(; *format && buff < end_buf; ++format)
	{
		switch(tok_type)         
		{
			case NMEA_TOKS_COMPARE:
				if('%' == *format)
					tok_type = NMEA_TOKS_PERCENT;
				else if(*buff++ != *format)
					goto fail;
				break;
			case NMEA_TOKS_PERCENT:
				width = 0;
				beg_fmt = format;
				tok_type = NMEA_TOKS_WIDTH;
      case NMEA_TOKS_WIDTH:
        if(isdigit(*format))
					break;                 
				{
					tok_type = NMEA_TOKS_TYPE;                     
					if(format > beg_fmt)
						width = nmea_atoi(beg_fmt, (int)(format - beg_fmt), 10);
				}
			case NMEA_TOKS_TYPE:
				beg_tok = buff;
				if(!width && ('c' == *format || 'C' == *format) && *buff != format[1])
					width = 1;
				
				if(width)
				{
					if(buff + width <= end_buf)
						buff += width;
					else
						goto fail;
				}
				else
				{
					if(!format[1] || (0 == (buff = (char *)memchr(buff, format[1], end_buf - buff))))
						buff = end_buf;
				}
				
				if(buff > end_buf)
					goto fail;
				
				tok_type = NMEA_TOKS_COMPARE;
				tok_count++;
				parg_target = 0;
				width = (int)(buff - beg_tok);
				
				switch(*format)                 
				{
					case 'c':
					case 'C':
						parg_target = (void *)va_arg(arg_ptr, char *);
						if(width && 0 != (parg_target))
							*((char *)parg_target) = *beg_tok;
						break;                     
					case 's':
					case 'S':                         
						parg_target = (void *)va_arg(arg_ptr, char *);
						if(width && 0 != (parg_target))
						{
							memcpy(parg_target, beg_tok, width);
							((char *)parg_target)[width] = '\0';
						}
						break;
					case 'f':
					case 'g':
					case 'G':
					case 'e':
					case 'E':
						parg_target = (void *)va_arg(arg_ptr, double *);
						if(width && 0 != (parg_target))
							*((double *)parg_target) = nmea_atof(beg_tok, width);
						break;
				};
				
				if(parg_target)
					break;
				
				if(0 == (parg_target = (void *)va_arg(arg_ptr, int *)))
					break;
				
				if(!width)
					break;
				
				switch(*format)
				{
					case 'd':
					case 'i':
						snum = nmea_atoi(beg_tok, width, 10);
						memcpy(parg_target, &snum, sizeof(int));
						break;
					case 'u':
						unum = nmea_atoi(beg_tok, width, 10);
						memcpy(parg_target, &unum, sizeof(unsigned int));
						break;
					case 'x':
					case 'X':
						unum = nmea_atoi(beg_tok, width, 16);
						memcpy(parg_target, &unum, sizeof(unsigned int));
						break;
					case 'o':
						unum = nmea_atoi(beg_tok, width, 8);
						memcpy(parg_target, &unum, sizeof(unsigned int));
						break;
					default:
						goto fail;
				};
				break;         
		};    
	} 
fail:
	va_end(arg_ptr);
	return tok_count; 
} 



void nmea_error(const char *str, ...) 
{
	int size;
	va_list arg_list;
	char buff[NMEA_DEF_PARSEBUFF];
	nmeaErrorFunc func = nmea_property()->error_func;
	if(func)
	{
		va_start(arg_list, str);
		size = NMEA_POSIX(vsnprintf)(&buff[0], NMEA_DEF_PARSEBUFF - 1, str, arg_list);
		va_end(arg_list);
		if(size > 0)
			(*func)(&buff[0], size);
	}
}
int _nmea_parse_time(const char *buff, int buff_sz, nmeaTIME *res)
{
	int success = 0;
	switch(buff_sz)
	{
		case sizeof("hhmmss") - 1:
			success = (3 == nmea_scanf(buff, buff_sz,"%2d%2d%2d", &(res->hour), &(res->min), &(res->sec)));
			break;
		case sizeof("hhmmss.s") - 1:
		case sizeof("hhmmss.ss") - 1:
		case sizeof("hhmmss.sss") - 1:
			success = (4 == nmea_scanf(buff, buff_sz,"%2d%2d%2d.%d", &(res->hour), &(res->min), &(res->sec), &(res->hsec)));
			break;
		default:
			nmea_error("Parse of time error (format error)!");
			success = 0;
			break;
	}
return (success ? 0 : -1);
} 
/**
* \brief Parse RMC packet from buffer. 
* @param buff a constant character pointer of packet buffer. 
* @param buff_sz buffer size. 
* @param pack a pointer of packet which will filled by function. 
* @return 1 (true) - if parsed successfully or 0 (false) - if fail. 
*/ 
int nmea_parse_GPRMC(const char *buff, int buff_sz, nmeaGPRMC *pack) 
{
	int nsen;
	char time_buff[NMEA_TIMEPARSE_BUF];
	NMEA_ASSERT(buff && pack);
	memset(pack, 0, sizeof(nmeaGPRMC));
	nmea_trace_buff(buff, buff_sz);
	nsen = nmea_scanf(buff, buff_sz,"$GNRMC,%s,%C,%f,%C,%f,%C,%f,%f,%2d%2d%2d,%f,%C,%C*",
																	&(time_buff[0]),
																	&(pack->status), &(pack->lat), &(pack->ns), &(pack->lon), &(pack->ew),
																	&(pack->speed), &(pack->direction),
																	&(pack->utc.day), &(pack->utc.mon), &(pack->utc.year),
																	&(pack->declination), &(pack->declin_ew), &(pack->mode));
	if(nsen != 13 && nsen != 14)
	{
		nmea_error("GNRMC parse error!");
		return 0;
	}

	if(0 != _nmea_parse_time(&time_buff[0], (int)strlen(&time_buff[0]), &(pack->utc)))
	{
		nmea_error("GNRMC time parse error!");
		return 0;
	}

	if(pack->utc.year < 90)
		pack->utc.year += 100;
	pack->utc.mon -= 1;
	
	return 1; 
} 



void nmea_time_now(nmeaTIME *stm) 
{
	time_t t;
	struct tm st;
	t = time(NULL);
	localtime_r(&t, &st);
	stm->year = st.tm_year;
	stm->mon = st.tm_mon;
	stm->day = st.tm_mday;
	stm->hour = st.tm_hour;
	stm->min = st.tm_min;
	stm->sec = st.tm_sec;
	stm->hsec = 0; 
} 

void nmea_zero_INFO(nmeaINFO *info) 
{
	memset(info, 0, sizeof(nmeaINFO));
	nmea_time_now(&info->utc);
	info->sig = NMEA_SIG_BAD;
	info->fix = NMEA_FIX_BAD;
	info->smask = 0; 
} 



/** 
* \brief Delete top packet from parser 
* @return Deleted packet type 
* @see nmeaPACKTYPE 
*/ 
int nmea_parser_drop(nmeaPARSER *parser) 
{
	int retval = GPNON;
	nmeaParserNODE *node = (nmeaParserNODE *)parser->top_node;
	NMEA_ASSERT(parser && parser->buffer);
	if(node)
	{
		if(node->pack)
		rt_free(node->pack);
		retval = node->packType;
		parser->top_node = node->next_node;
		if(!parser->top_node)
		parser->end_node = 0;
		rt_free(node);
	}
	return retval;
}
/** 
* \brief Initialization of parser object 
* @return true (1) - success or false (0) - fail 
*/ 
int nmea_parser_init(nmeaPARSER *parser) 
{
	int resv = 0;
	int buff_size = nmea_property()->parse_buff_size;
	NMEA_ASSERT(parser);
	if(buff_size < NMEA_MIN_PARSEBUFF)
		buff_size = NMEA_MIN_PARSEBUFF;
	memset(parser, 0, sizeof(nmeaPARSER));

	if(0 == (parser->buffer = rt_malloc(buff_size)))
		nmea_error("Insufficient memory!");
	else
	{
		parser->buff_size = buff_size;
		resv = 1;
	}
	return resv; 
} 

/** 
* \brief Clear packets queue into parser 
* @return true (1) - success */ 
int nmea_parser_queue_clear(nmeaPARSER *parser)
{
	NMEA_ASSERT(parser);
	while(parser->top_node)
	nmea_parser_drop(parser);
	return 1; 
} 

/** 
* \brief Destroy parser object 
*/ 
void nmea_parser_destroy(nmeaPARSER *parser) 
{
	NMEA_ASSERT(parser && parser->buffer);
	rt_free(parser->buffer);
	nmea_parser_queue_clear(parser);
	memset(parser, 0, sizeof(nmeaPARSER)); 
} 

/** 
* \brief Clear cache of parser 
* @return true (1) - success 
*/ 
int nmea_parser_buff_clear(nmeaPARSER *parser) 
{
	NMEA_ASSERT(parser && parser->buffer);
	parser->buff_use = 0;
	return 1; 
} 

/** 
* \brief Find tail of packet ("\r\n") in buffer and check control sum (CRC). 
* @param buff a constant character pointer of packets buffer. 
* @param buff_sz buffer size. 
* @param res_crc a integer pointer for return CRC of packet (must be defined). 
* @return Number of bytes to packet tail. */ 
int nmea_find_tail(const char *buff, int buff_sz, int *res_crc) 
{
	static const int tail_sz = 3 /* *[CRC] */ + 2 /* \r\n */;    
	const char *end_buff = buff + buff_sz;
	int nread = 0;
	int crc = 0;
	NMEA_ASSERT(buff && res_crc);
	*res_crc = -1;
	
	for(; buff < end_buff; ++buff, ++nread)
	{
		if(('$' == *buff) && nread)
		{
			buff = 0;
			break;
		}
		else if('*' == *buff)
		{
			if(buff + tail_sz <= end_buff && '\r' == buff[3] && '\n' == buff[4])
			{
				*res_crc = nmea_atoi(buff + 1, 2, 16);
				nread = buff_sz - (int)(end_buff - (buff + tail_sz));
				if(*res_crc != crc)
				{
					*res_crc = -1;
					buff = 0;
				}
			}
			break;
		}
		else if(nread)
			crc ^= (int) * buff;
	}

	if(*res_crc < 0 && buff)
		nread = 0;
	return nread; 
} 

/** 
* \brief Define packet type by header (nmeaPACKTYPE). 
* @param buff a constant character pointer of packet buffer. 
* @param buff_sz buffer size. 
* @return The defined packet type 
* @see nmeaPACKTYPE 
*/ 
int nmea_pack_type(const char *buff, int buff_sz)
{
	static const char *pheads[] =
															{
																"GPGGA",
																"GPGSA",
																"GPGSV",
																"GPRMC",
																"GPVTG",
															};
	NMEA_ASSERT(buff);
															
	if(buff_sz < 5)
		return GPNON;
	else if(0 == memcmp(buff, pheads[0], 5))
		return GPGGA;
	else if(0 == memcmp(buff, pheads[1], 5))
		return GPGSA;
	else if(0 == memcmp(buff, pheads[2], 5))
		return GPGSV;
	else if(0 == memcmp(buff, pheads[3], 5))
		return GPRMC;
	else if(0 == memcmp(buff, pheads[4], 5))
		return GPVTG;

	return GPNON; 
} 

int nmea_parser_real_push(nmeaPARSER *parser, const char *buff, int buff_sz) 
{
	int nparsed = 0, crc, sen_sz, ptype;
	nmeaParserNODE *node = 0;
	NMEA_ASSERT(parser && parser->buffer);
	/* clear unuse buffer (for debug) */
	/*
	memset(
		parser->buffer + parser->buff_use, 0,
		parser->buff_size - parser->buff_use
	);
	*/
	/* add */
	if(parser->buff_use + buff_sz >= parser->buff_size)
		nmea_parser_buff_clear(parser);
	
	memcpy(parser->buffer + parser->buff_use, buff, buff_sz);
	parser->buff_use += buff_sz;
	/* parse */
	for(;; node = 0)
	{
		sen_sz = nmea_find_tail(
														(const char *)parser->buffer + nparsed,
														(int)parser->buff_use - nparsed, &crc);
		if(!sen_sz)
		{
			if(nparsed)
			memcpy(parser->buffer,parser->buffer + nparsed,parser->buff_use -= nparsed);
			break;
		}
		else if(crc >= 0)
		{
			ptype = nmea_pack_type((const char *)parser->buffer + nparsed + 1,parser->buff_use - nparsed - 1);
			if(0 == (node = rt_malloc(sizeof(nmeaParserNODE))))
				goto mem_fail;
			
			node->pack = 0;
			
			switch(ptype)
			{
				case GPGGA:
					rt_free(node);
					node = 0;
					break;
				case GPGSA:
					rt_free(node);
					node = 0;
					break;
				case GPGSV:
					rt_free(node);
					node = 0;
					break;
				case GPRMC:
					if(0 == (node->pack = rt_malloc(sizeof(nmeaGPRMC))))
						goto mem_fail;
					node->packType = GPRMC;
					if(!nmea_parse_GPRMC((const char *)parser->buffer + nparsed,sen_sz, (nmeaGPRMC *)node->pack))
					{
						rt_free(node);
						node = 0;
					}
					break;
				case GPVTG:
						break;
				default:
					rt_free(node);
					node = 0;
					break;
			};
			
			if(node)
			{
				if(parser->end_node)
					((nmeaParserNODE *)parser->end_node)->next_node = node;
				parser->end_node = node;
				if(!parser->top_node)
					parser->top_node = node;
				node->next_node = 0;
			}
		}

		nparsed += sen_sz;
	}
	return nparsed; 
mem_fail:
	if(node)
		rt_free(node);
		nmea_error("Insufficient memory!");
return -1; } 

/** 
* \brief Analysis of buffer and keep results into parser 
* @return Number of bytes wos parsed from buffer 
*/ 
int nmea_parser_push(nmeaPARSER *parser, const char *buff, int buff_sz) 
{
	int nparse, nparsed = 0;
	do
	{
		if(buff_sz > parser->buff_size)
			nparse = parser->buff_size;
		else
			nparse = buff_sz;
			nparsed += nmea_parser_real_push(parser, buff, nparse);
		buff_sz -= nparse;
	}while(buff_sz);
	
return nparsed; 
} 



/** 
* \brief Fill nmeaINFO structure by RMC packet data. 
* @param pack a pointer of packet structure. 
* @param info a pointer of summary information structure. 
*/ 
void nmea_GPRMC2info(nmeaGPRMC *pack, nmeaINFO *info) 
{
	NMEA_ASSERT(pack && info);
	if('A' == pack->status)
	{
		if(NMEA_SIG_BAD == info->sig)
			info->sig = NMEA_SIG_MID;
		if(NMEA_FIX_BAD == info->fix)
			info->fix = NMEA_FIX_2D;
	}
	else if('V' == pack->status)
	{
		info->sig = NMEA_SIG_BAD;
		info->fix = NMEA_FIX_BAD;
	}
	
	info->utc = pack->utc;
	info->lat = ((pack->ns == 'N') ? pack->lat : -(pack->lat));
	info->lon = ((pack->ew == 'E') ? pack->lon : -(pack->lon));
	info->speed = pack->speed * NMEA_TUD_KNOTS;
	info->direction = pack->direction;
	info->smask |= GPRMC; 
} 

/** 
* \brief Withdraw top packet from parser 
* @return Received packet type 
* @see nmeaPACKTYPE 
*/ 
int nmea_parser_pop(nmeaPARSER *parser, void **pack_ptr) 
{
	int retval = GPNON;
	nmeaParserNODE *node = (nmeaParserNODE *)parser->top_node;
	NMEA_ASSERT(parser && parser->buffer);
	
	if(node)
	{
		*pack_ptr = node->pack;
		retval = node->packType;
		parser->top_node = node->next_node;
		
		if(!parser->top_node)
			parser->end_node = 0;
		
		rt_free(node);
	}
	return retval; 
} 

/** 
* \brief Analysis of buffer and put results to information structure 
* @return Number of packets wos parsed 
*/ 
int nmea_parse(
	nmeaPARSER *parser,
	const char *buff, int buff_sz,
	nmeaINFO *info )
{
	int ptype, nread = 0;
	void *pack = 0;
	NMEA_ASSERT(parser && parser->buffer);
	nmea_parser_push(parser, buff, buff_sz);
	while(GPNON != (ptype = nmea_parser_pop(parser, &pack)))
	{
		nread++;
		switch(ptype)
		{
			case GPGGA:
				break;
			case GPGSA:
				break;
			case GPGSV:
				break;
			case GPRMC:
				nmea_GPRMC2info((nmeaGPRMC *)pack, info);
				break;
			case GPVTG:
				break;
		};
		rt_free(pack);
	}
	return nread;
}
