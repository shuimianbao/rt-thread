#ifndef _GPRMC_H_
#define _GPRMC_H_

/** 
  * Date and time data 
  * @see nmea_time_now 
*/ 
typedef struct _nmeaTIME 
{     
	int     year;       /**< Years since 1900 */     
	int     mon;        /**< Months since January - [0,11] */     
	int     day;        /**< Day of the month - [1,31] */     
	int     hour;       /**< Hours since midnight - [0,23] */     
	int     min;        /**< Minutes after the hour - [0,59] */     
	int     sec;        /**< Seconds after the minute - [0,59] */    
	int     hsec;       /**< Hundredth part of second - [0,99] */ 
} nmeaTIME; 

/** 
  * RMC packet information structure (Recommended Minimum sentence C) 
*/ 
typedef struct _nmeaGPRMC 
{     
	nmeaTIME utc;       /**< UTC of position */     
	char    status;     /**< Status (A = active or V = void) */     
	double  lat;        /**< Latitude in NDEG - [degree][min].[sec/60] */     
	char    ns;         /**< [N]orth or [S]outh */     
	double  lon;        /**< Longitude in NDEG - [degree][min].[sec/60] */     
	char    ew;         /**< [E]ast or [W]est */     
	double  speed;      /**< Speed over the ground in knots */     
	double  direction;  /**< Track angle in degrees True */     
	double  declination; /**< Magnetic variation degrees (Easterly var. subtracts from true course) */
	char    declin_ew;  /**< [E]ast or [W]est */
	char    mode;       /**< Mode indicator of fix type (A = autonomous, D = differential, E = estimated, N = not valid, S = simulator) */
} nmeaGPRMC; 

#define NMEA_CONVSTR_BUF    (256) 
#define NMEA_TIMEPARSE_BUF  (256) 
#define NMEA_DEF_PARSEBUFF  (1024) 
#define NMEA_MIN_PARSEBUFF  (256) 
#define NMEA_ASSERT(x)   RT_ASSERT(x) 

typedef void(*nmeaTraceFunc)(const char *str, int str_size); 
typedef void(*nmeaErrorFunc)(const char *str, int str_size); 

typedef struct _nmeaPROPERTY 
{
     nmeaTraceFunc   trace_func;
     nmeaErrorFunc   error_func;
     int             parse_buff_size;
} nmeaPROPERTY;

#define NMEA_TOKS_COMPARE   (1) 
#define NMEA_TOKS_PERCENT   (2) 
#define NMEA_TOKS_WIDTH     (3) 
#define NMEA_TOKS_TYPE      (4) 

#if defined(_MSC_VER) 
	# define NMEA_POSIX(x)  _##x 
	# define NMEA_INLINE    __inline 
#else 
	# define NMEA_POSIX(x)  x 
	# define NMEA_INLINE    inline 
#endif 

#define NMEA_SIG_BAD        (0) 
#define NMEA_SIG_LOW        (1) 
#define NMEA_SIG_MID        (2) 
#define NMEA_SIG_HIGH       (3)

#define NMEA_FIX_BAD        (1) 
#define NMEA_FIX_2D         (2) 
#define NMEA_FIX_3D         (3)

#define NMEA_MAXSAT         (12) 
#define NMEA_SATINPACK      (4) 
#define NMEA_NSATPACKS      (NMEA_MAXSAT / NMEA_SATINPACK) 
#define NMEA_DEF_LAT        (5001.2621) 
#define NMEA_DEF_LON        (3613.0595) 

/** 
* Position data in fractional degrees or radians 
*/
typedef struct _nmeaPOS 
{
	double lat;         /**< Latitude */
	double lon;         /**< Longitude */ 
} nmeaPOS; 

/** 
* Information about satellite 
* @see nmeaSATINFO 
* @see nmeaGPGSV 
*/ 
typedef struct _nmeaSATELLITE 
{
	int     id;         /**< Satellite PRN number */
	int     in_use;     /**< Used in position fix */
	int     elv;        /**< Elevation in degrees, 90 maximum */
	int     azimuth;    /**< Azimuth, degrees from true north, 000 to 359 */
	int     sig;        /**< Signal, 00-99 dB */ 
} nmeaSATELLITE; 

/** 
* Information about all satellites in view 
* @see nmeaINFO 
* @see nmeaGPGSV 
*/ 
typedef struct _nmeaSATINFO 
{
	int     inuse;      /**< Number of satellites in use (not those in view) */
	int     inview;     /**< Total number of satellites in view */
	nmeaSATELLITE sat[NMEA_MAXSAT]; /**< Satellites information */ 
} nmeaSATINFO; 

/**
* Summary GPS information from all parsed packets, 
* used also for generating NMEA stream 
* @see nmea_parse 
* @see nmea_GPGGA2info,  nmea_...2info 
*/ 
typedef struct _nmeaINFO 
{
	int     smask;      /**< Mask specifying types of packages from which data have been obtained */
	nmeaTIME utc;       /**< UTC of position */     
	int     sig;        /**< GPS quality indicator (0 = Invalid; 1 = Fix; 2 = Differential, 3 = Sensitive) */
	int     fix;        /**< Operating mode, used for navigation (1 = Fix not available; 2 = 2D; 3 = 3D) */
	double  PDOP;       /**< Position Dilution Of Precision */
	double  HDOP;       /**< Horizontal Dilution Of Precision */
	double  VDOP;       /**< Vertical Dilution Of Precision */
	double  lat;        /**< Latitude in NDEG - +/-[degree][min].[sec/60] */
	double  lon;        /**< Longitude in NDEG - +/-[degree][min].[sec/60] */
	double  elv;        /**< Antenna altitude above/below mean sea level (geoid) in meters */
	double  speed;      /**< Speed over the ground in kilometers/hour */
	double  direction;  /**< Track angle in degrees True */
	double  declination; /**< Magnetic variation degrees (Easterly var. subtracts from true course) */
	nmeaSATINFO satinfo; /**< Satellites information */ 
} nmeaINFO; 

/* 
* high level 
*/ 
typedef struct _nmeaPARSER 
{
	void *top_node;
	void *end_node;
	unsigned char *buffer;
	int buff_size;
	int buff_use;
} nmeaPARSER; 

/** 
* NMEA packets type which parsed and generated by library 
*/ 
enum nmeaPACKTYPE 
{
	GPNON = 0x0000,   /**< Unknown packet type. */
	GPGGA = 0x0001,   /**< GGA - Essential fix data which provide 3D location and accuracy data. */
	GPGSA = 0x0002,   /**< GSA - GPS receiver operating mode, SVs used for navigation, and DOP values. */
	GPGSV = 0x0004,   /**< GSV - Number of SVs in view, PRN numbers, elevation, azimuth & SNR values. */
	GPRMC = 0x0008,   /**< RMC - Recommended Minimum Specific GPS/TRANSIT Data. */
	GPVTG = 0x0010    /**< VTG - Actual track made good and speed over ground. */ 
}; 

typedef struct _nmeaParserNODE 
{
	int packType;
	void *pack;
	struct _nmeaParserNODE *next_node; 
} nmeaParserNODE; 

#define NMEA_TUD_YARDS      (1.0936)        /**< Yeards, meter * NMEA_TUD_YARDS = yard */ 
#define NMEA_TUD_KNOTS      (1.852)         /**< Knots, kilometer / NMEA_TUD_KNOTS = knot */ 
#define NMEA_TUD_MILES      (1.609)         /**< Miles, kilometer / NMEA_TUD_MILES = mile */ 

/* 
* Speed units 
*/ 
#define NMEA_TUS_MS         (3.6)           /**< Meters per seconds, (k/h) / NMEA_TUS_MS= (m/s) */ 

nmeaPROPERTY * nmea_property(void); 
void nmea_trace_buff(const char *buff, int buff_size);
int nmea_atoi(const char *str, int str_sz, int radix);
double nmea_atof(const char *str, int str_sz);
int nmea_scanf(const char *buff, int buff_sz, const char *format, ...);
void nmea_error(const char *str, ...) ;
int _nmea_parse_time(const char *buff, int buff_sz, nmeaTIME *res);
int nmea_parse_GPRMC(const char *buff, int buff_sz, nmeaGPRMC *pack) ;
void nmea_time_now(nmeaTIME *stm) ;
void nmea_zero_INFO(nmeaINFO *info) ;
int nmea_parser_drop(nmeaPARSER *parser) ;
int nmea_parser_init(nmeaPARSER *parser) ;
int nmea_parser_queue_clear(nmeaPARSER *parser);
void nmea_parser_destroy(nmeaPARSER *parser) ;
int nmea_parser_buff_clear(nmeaPARSER *parser) ;
int nmea_find_tail(const char *buff, int buff_sz, int *res_crc) ;
int nmea_pack_type(const char *buff, int buff_sz);
int nmea_parser_real_push(nmeaPARSER *parser, const char *buff, int buff_sz) ;
int nmea_parser_push(nmeaPARSER *parser, const char *buff, int buff_sz) ;
void nmea_GPRMC2info(nmeaGPRMC *pack, nmeaINFO *info) ;
int nmea_parser_pop(nmeaPARSER *parser, void **pack_ptr) ;
int nmea_parse(	nmeaPARSER *parser,	char *buff, int buff_sz,	nmeaINFO *info );


#endif
