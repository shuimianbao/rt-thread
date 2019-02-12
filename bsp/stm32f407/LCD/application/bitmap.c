//=============================================================================
//             _   __    ______    ____       ______    ______
//            / | / /   / ____/   / __ \     / ____/   / ____/
//           /  |/ /   / __/     / / / /    / __/     / __/
//          / /|  /   / /___    / /_/ /    / /___    / /___
//         /_/ |_/   /_____/    \___\_\   /_____/   /_____/
//        (c)1996-2016 NEQEE Photoelectric Technology Co., Ltd.
//        Internet: www.neqee.com  Designer: Chen ZeHao
//=============================================================================

#include "GUI.h"

const GUI_BITMAP bmmov0_00 = {200, 450, 600, 24, "mov0/00", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_01 = {200, 450, 600, 24, "mov0/01", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_02 = {200, 450, 600, 24, "mov0/02", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_03 = {200, 450, 600, 24, "mov0/03", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_04 = {200, 450, 600, 24, "mov0/04", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_05 = {200, 450, 600, 24, "mov0/05", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_06 = {200, 450, 600, 24, "mov0/06", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_07 = {200, 450, 600, 24, "mov0/07", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_08 = {200, 450, 600, 24, "mov0/08", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_09 = {200, 450, 600, 24, "mov0/09", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_10 = {200, 450, 600, 24, "mov0/10", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_11 = {200, 450, 600, 24, "mov0/11", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_12 = {200, 450, 600, 24, "mov0/12", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_13 = {200, 450, 600, 24, "mov0/13", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_14 = {200, 450, 600, 24, "mov0/14", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_15 = {200, 450, 600, 24, "mov0/15", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_16 = {200, 450, 600, 24, "mov0/16", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_17 = {200, 450, 600, 24, "mov0/17", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_18 = {200, 450, 600, 24, "mov0/18", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_19 = {200, 450, 600, 24, "mov0/19", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_20 = {200, 450, 600, 24, "mov0/20", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_21 = {200, 450, 600, 24, "mov0/21", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_22 = {200, 450, 600, 24, "mov0/22", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmmov0_23 = {200, 450, 600, 24, "mov0/23", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmPageSKB_disb = {151, 227, 456, 24, "PageSKB_disb", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmPageSKB_mark = {151, 227, 456, 24, "PageSKB_mark", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmPageSKB_nor = {151, 227, 456, 24, "PageSKB_nor", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmWindowDLG1_mark = {800, 480, 2400, 24, "WindowDLG1_mark", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmWindowDLG1_nor = {800, 480, 2400, 24, "WindowDLG1_nor", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmWindowDLG1_thumbM = {14, 28, 44, 24, "WindowDLG1_thumbM", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmWindowDLG1_thumbN = {14, 28, 44, 24, "WindowDLG1_thumbN", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmWindowDLG2_mark = {800, 480, 2400, 24, "WindowDLG2_mark", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmWindowDLG2_nor = {800, 480, 2400, 24, "WindowDLG2_nor", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmcranes_m = {800, 600, 2400, 24, "cranes_m", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmcranes_n = {800, 600, 2400, 24, "cranes_n", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmwelcome_m = {800, 600, 2400, 24, "welcome_m", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmwelcome_n = {800, 600, 2400, 24, "welcome_n", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmgps0 = {800, 600, 2400, 24, "gps0", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmgps1 = {800, 600, 2400, 24, "gps1", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmsignal0 = {800, 600, 2400, 24, "signal0", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmsignal1 = {800, 600, 2400, 24, "signal1", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmsignal2 = {800, 600, 2400, 24, "signal2", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmsignal3 = {800, 600, 2400, 24, "signal3", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmsignal4 = {800, 600, 2400, 24, "signal4", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmsignal5 = {800, 600, 2400, 24, "signal5", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmsafelight = {800, 600, 2400, 24, "safelight", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmdangerlight = {800, 600, 2400, 24, "dangerlight", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmcranescfg_n = {800, 600, 2400, 24, "cranes-cfg_n", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmcranescfg_m = {800, 600, 2400, 24, "cranes-cfg_m", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmskb_n = {800, 600, 2400, 24, "SKB_n", 0, GUI_DRAW_BMPTR};
const GUI_BITMAP bmskb_m = {800, 600, 2400, 24, "SKB_m", 0, GUI_DRAW_BMPTR};