/*
 * Copyright (C) 2018 xiaoliang<1296283984@qq.com>.
 */

/************************************************************************
 * BP STMT (breakpoint statement block), a programming method that sets a breakpoint
 * in a statement block and resumes on the next enter.
 * BP STMT(breakpoint statement block)，一种可在语句块中设置断点，且在下次进入时恢复的编程方式
 ************************************************************************
 */

#ifndef __BP_STMT_H__
#define __BP_STMT_H__

/*************************************************************************
 * BP_STMT macro build
 * BP_STMT 宏构造
 *************************************************************************
 */

/* BP_STMT macro connection, and fully expand */
/* BP_STMT宏连接，并完全展开 */
#define __BP_STMT_CAT(x, y)  __BP_STMT_CAT1(x, y)
#define __BP_STMT_CAT1(x, y) x##y

/* BP_STMT label with N suffix */
/* 以N为后缀的BP_STMT标签 */
#define BP_STMT_LABEL_N(prefix, n)           __BP_STMT_CAT(prefix, n)

/* BP_STMT start label */
/* BP_STMT开始标签 */
#define BP_STMT_LABEL_START(prefix)          BP_STMT_LABEL_N(prefix, 0)

/* BP_STMT end label */
/* BP_STMT结束标签 */
#define BP_STMT_LABEL_END(prefix)            BP_STMT_LABEL_N(prefix, end)

/* the head of the BP_STMT_BEGIN,
 * the switch-goto statement block */
/* BP_STMT_BEGIN的头部，switch-goto语句块 */
#define __BP_STMT_HEADER(bp)        do {                                        \
                                    switch ((bp))                               \
                                    {                                           \

/* the tail of the BP_STMT_BEGIN,
 * the switch-goto statement block */
/* BP_STMT_BEGIN的尾部，switch-goto语句块 */
#define __BP_STMT_TAIL(prefix)          default:                                \
                                        goto BP_STMT_LABEL_END(prefix);         \
                                    }                                           \
                                    BP_STMT_LABEL_START(prefix):;               \
                                } while (0); if(1)                              \

/* the block part of the case statement for BP_STMT_BEGIN */
/* BP_STMT_BEGIN的case语句块部分 */
#define __BP_STMT_CASE0(prefix)      case 0: goto BP_STMT_LABEL_START(prefix);
#define __BP_STMT_CASE1(prefix)      __BP_STMT_CASE0(prefix) case 1: goto BP_STMT_LABEL_N(prefix, 1);
#define __BP_STMT_CASE2(prefix)      __BP_STMT_CASE1(prefix) case 2: goto BP_STMT_LABEL_N(prefix, 2);
#define __BP_STMT_CASE3(prefix)      __BP_STMT_CASE2(prefix) case 3: goto BP_STMT_LABEL_N(prefix, 3);
#define __BP_STMT_CASE4(prefix)      __BP_STMT_CASE3(prefix) case 4: goto BP_STMT_LABEL_N(prefix, 4);
#define __BP_STMT_CASE5(prefix)      __BP_STMT_CASE4(prefix) case 5: goto BP_STMT_LABEL_N(prefix, 5);
#define __BP_STMT_CASE6(prefix)      __BP_STMT_CASE5(prefix) case 6: goto BP_STMT_LABEL_N(prefix, 6);
#define __BP_STMT_CASE7(prefix)      __BP_STMT_CASE6(prefix) case 7: goto BP_STMT_LABEL_N(prefix, 7);
#define __BP_STMT_CASE8(prefix)      __BP_STMT_CASE7(prefix) case 8: goto BP_STMT_LABEL_N(prefix, 8);
#define __BP_STMT_CASE9(prefix)      __BP_STMT_CASE8(prefix) case 9: goto BP_STMT_LABEL_N(prefix, 9);
#define __BP_STMT_CASE10(prefix)     __BP_STMT_CASE9(prefix) case 10: goto BP_STMT_LABEL_N(prefix, 10);
#define __BP_STMT_CASE11(prefix)     __BP_STMT_CASE10(prefix) case 11: goto BP_STMT_LABEL_N(prefix, 11);
#define __BP_STMT_CASE12(prefix)     __BP_STMT_CASE11(prefix) case 12: goto BP_STMT_LABEL_N(prefix, 12);
#define __BP_STMT_CASE13(prefix)     __BP_STMT_CASE12(prefix) case 13: goto BP_STMT_LABEL_N(prefix, 13);
#define __BP_STMT_CASE14(prefix)     __BP_STMT_CASE13(prefix) case 14: goto BP_STMT_LABEL_N(prefix, 14);
#define __BP_STMT_CASE15(prefix)     __BP_STMT_CASE14(prefix) case 15: goto BP_STMT_LABEL_N(prefix, 15);
#define __BP_STMT_CASE16(prefix)     __BP_STMT_CASE15(prefix) case 16: goto BP_STMT_LABEL_N(prefix, 16);
#define __BP_STMT_CASE17(prefix)     __BP_STMT_CASE16(prefix) case 17: goto BP_STMT_LABEL_N(prefix, 17);
#define __BP_STMT_CASE18(prefix)     __BP_STMT_CASE17(prefix) case 18: goto BP_STMT_LABEL_N(prefix, 18);
#define __BP_STMT_CASE19(prefix)     __BP_STMT_CASE18(prefix) case 19: goto BP_STMT_LABEL_N(prefix, 19);
#define __BP_STMT_CASE20(prefix)     __BP_STMT_CASE19(prefix) case 20: goto BP_STMT_LABEL_N(prefix, 20);
#define __BP_STMT_CASE21(prefix)     __BP_STMT_CASE20(prefix) case 21: goto BP_STMT_LABEL_N(prefix, 21);
#define __BP_STMT_CASE22(prefix)     __BP_STMT_CASE21(prefix) case 22: goto BP_STMT_LABEL_N(prefix, 22);
#define __BP_STMT_CASE23(prefix)     __BP_STMT_CASE22(prefix) case 23: goto BP_STMT_LABEL_N(prefix, 23);
#define __BP_STMT_CASE24(prefix)     __BP_STMT_CASE23(prefix) case 24: goto BP_STMT_LABEL_N(prefix, 24);
#define __BP_STMT_CASE25(prefix)     __BP_STMT_CASE24(prefix) case 25: goto BP_STMT_LABEL_N(prefix, 25);
#define __BP_STMT_CASE26(prefix)     __BP_STMT_CASE25(prefix) case 26: goto BP_STMT_LABEL_N(prefix, 26);
#define __BP_STMT_CASE27(prefix)     __BP_STMT_CASE26(prefix) case 27: goto BP_STMT_LABEL_N(prefix, 27);
#define __BP_STMT_CASE28(prefix)     __BP_STMT_CASE27(prefix) case 28: goto BP_STMT_LABEL_N(prefix, 28);
#define __BP_STMT_CASE29(prefix)     __BP_STMT_CASE28(prefix) case 29: goto BP_STMT_LABEL_N(prefix, 29);
#define __BP_STMT_CASE30(prefix)     __BP_STMT_CASE29(prefix) case 30: goto BP_STMT_LABEL_N(prefix, 30);
#define __BP_STMT_CASE31(prefix)     __BP_STMT_CASE30(prefix) case 31: goto BP_STMT_LABEL_N(prefix, 31);
#define __BP_STMT_CASE32(prefix)     __BP_STMT_CASE31(prefix) case 32: goto BP_STMT_LABEL_N(prefix, 32);
#define __BP_STMT_CASE33(prefix)     __BP_STMT_CASE32(prefix) case 33: goto BP_STMT_LABEL_N(prefix, 33);
#define __BP_STMT_CASE34(prefix)     __BP_STMT_CASE33(prefix) case 34: goto BP_STMT_LABEL_N(prefix, 34);
#define __BP_STMT_CASE35(prefix)     __BP_STMT_CASE34(prefix) case 35: goto BP_STMT_LABEL_N(prefix, 35);
#define __BP_STMT_CASE36(prefix)     __BP_STMT_CASE35(prefix) case 36: goto BP_STMT_LABEL_N(prefix, 36);
#define __BP_STMT_CASE37(prefix)     __BP_STMT_CASE36(prefix) case 37: goto BP_STMT_LABEL_N(prefix, 37);
#define __BP_STMT_CASE38(prefix)     __BP_STMT_CASE37(prefix) case 38: goto BP_STMT_LABEL_N(prefix, 38);
#define __BP_STMT_CASE39(prefix)     __BP_STMT_CASE38(prefix) case 39: goto BP_STMT_LABEL_N(prefix, 39);
#define __BP_STMT_CASE40(prefix)     __BP_STMT_CASE39(prefix) case 40: goto BP_STMT_LABEL_N(prefix, 40);
#define __BP_STMT_CASE41(prefix)     __BP_STMT_CASE40(prefix) case 41: goto BP_STMT_LABEL_N(prefix, 41);
#define __BP_STMT_CASE42(prefix)     __BP_STMT_CASE41(prefix) case 42: goto BP_STMT_LABEL_N(prefix, 42);
#define __BP_STMT_CASE43(prefix)     __BP_STMT_CASE42(prefix) case 43: goto BP_STMT_LABEL_N(prefix, 43);
#define __BP_STMT_CASE44(prefix)     __BP_STMT_CASE43(prefix) case 44: goto BP_STMT_LABEL_N(prefix, 44);
#define __BP_STMT_CASE45(prefix)     __BP_STMT_CASE44(prefix) case 45: goto BP_STMT_LABEL_N(prefix, 45);
#define __BP_STMT_CASE46(prefix)     __BP_STMT_CASE45(prefix) case 46: goto BP_STMT_LABEL_N(prefix, 46);
#define __BP_STMT_CASE47(prefix)     __BP_STMT_CASE46(prefix) case 47: goto BP_STMT_LABEL_N(prefix, 47);
#define __BP_STMT_CASE48(prefix)     __BP_STMT_CASE47(prefix) case 48: goto BP_STMT_LABEL_N(prefix, 48);
#define __BP_STMT_CASE49(prefix)     __BP_STMT_CASE48(prefix) case 49: goto BP_STMT_LABEL_N(prefix, 49);
#define __BP_STMT_CASE50(prefix)     __BP_STMT_CASE49(prefix) case 50: goto BP_STMT_LABEL_N(prefix, 50);
#define __BP_STMT_CASE51(prefix)     __BP_STMT_CASE50(prefix) case 51: goto BP_STMT_LABEL_N(prefix, 51);
#define __BP_STMT_CASE52(prefix)     __BP_STMT_CASE51(prefix) case 52: goto BP_STMT_LABEL_N(prefix, 52);
#define __BP_STMT_CASE53(prefix)     __BP_STMT_CASE52(prefix) case 53: goto BP_STMT_LABEL_N(prefix, 53);
#define __BP_STMT_CASE54(prefix)     __BP_STMT_CASE53(prefix) case 54: goto BP_STMT_LABEL_N(prefix, 54);
#define __BP_STMT_CASE55(prefix)     __BP_STMT_CASE54(prefix) case 55: goto BP_STMT_LABEL_N(prefix, 55);
#define __BP_STMT_CASE56(prefix)     __BP_STMT_CASE55(prefix) case 56: goto BP_STMT_LABEL_N(prefix, 56);
#define __BP_STMT_CASE57(prefix)     __BP_STMT_CASE56(prefix) case 57: goto BP_STMT_LABEL_N(prefix, 57);
#define __BP_STMT_CASE58(prefix)     __BP_STMT_CASE57(prefix) case 58: goto BP_STMT_LABEL_N(prefix, 58);
#define __BP_STMT_CASE59(prefix)     __BP_STMT_CASE58(prefix) case 59: goto BP_STMT_LABEL_N(prefix, 59);
#define __BP_STMT_CASE60(prefix)     __BP_STMT_CASE59(prefix) case 60: goto BP_STMT_LABEL_N(prefix, 60);
#define __BP_STMT_CASE61(prefix)     __BP_STMT_CASE60(prefix) case 61: goto BP_STMT_LABEL_N(prefix, 61);
#define __BP_STMT_CASE62(prefix)     __BP_STMT_CASE61(prefix) case 62: goto BP_STMT_LABEL_N(prefix, 62);
#define __BP_STMT_CASE63(prefix)     __BP_STMT_CASE62(prefix) case 63: goto BP_STMT_LABEL_N(prefix, 63);
#define __BP_STMT_CASE64(prefix)     __BP_STMT_CASE63(prefix) case 64: goto BP_STMT_LABEL_N(prefix, 64);
#define __BP_STMT_CASE65(prefix)     __BP_STMT_CASE64(prefix) case 65: goto BP_STMT_LABEL_N(prefix, 65);
#define __BP_STMT_CASE66(prefix)     __BP_STMT_CASE65(prefix) case 66: goto BP_STMT_LABEL_N(prefix, 66);
#define __BP_STMT_CASE67(prefix)     __BP_STMT_CASE66(prefix) case 67: goto BP_STMT_LABEL_N(prefix, 67);
#define __BP_STMT_CASE68(prefix)     __BP_STMT_CASE67(prefix) case 68: goto BP_STMT_LABEL_N(prefix, 68);
#define __BP_STMT_CASE69(prefix)     __BP_STMT_CASE68(prefix) case 69: goto BP_STMT_LABEL_N(prefix, 69);
#define __BP_STMT_CASE70(prefix)     __BP_STMT_CASE69(prefix) case 70: goto BP_STMT_LABEL_N(prefix, 70);
#define __BP_STMT_CASE71(prefix)     __BP_STMT_CASE70(prefix) case 71: goto BP_STMT_LABEL_N(prefix, 71);
#define __BP_STMT_CASE72(prefix)     __BP_STMT_CASE71(prefix) case 72: goto BP_STMT_LABEL_N(prefix, 72);
#define __BP_STMT_CASE73(prefix)     __BP_STMT_CASE72(prefix) case 73: goto BP_STMT_LABEL_N(prefix, 73);
#define __BP_STMT_CASE74(prefix)     __BP_STMT_CASE73(prefix) case 74: goto BP_STMT_LABEL_N(prefix, 74);
#define __BP_STMT_CASE75(prefix)     __BP_STMT_CASE74(prefix) case 75: goto BP_STMT_LABEL_N(prefix, 75);
#define __BP_STMT_CASE76(prefix)     __BP_STMT_CASE75(prefix) case 76: goto BP_STMT_LABEL_N(prefix, 76);
#define __BP_STMT_CASE77(prefix)     __BP_STMT_CASE76(prefix) case 77: goto BP_STMT_LABEL_N(prefix, 77);
#define __BP_STMT_CASE78(prefix)     __BP_STMT_CASE77(prefix) case 78: goto BP_STMT_LABEL_N(prefix, 78);
#define __BP_STMT_CASE79(prefix)     __BP_STMT_CASE78(prefix) case 79: goto BP_STMT_LABEL_N(prefix, 79);
#define __BP_STMT_CASE80(prefix)     __BP_STMT_CASE79(prefix) case 80: goto BP_STMT_LABEL_N(prefix, 80);
#define __BP_STMT_CASE81(prefix)     __BP_STMT_CASE80(prefix) case 81: goto BP_STMT_LABEL_N(prefix, 81);
#define __BP_STMT_CASE82(prefix)     __BP_STMT_CASE81(prefix) case 82: goto BP_STMT_LABEL_N(prefix, 82);
#define __BP_STMT_CASE83(prefix)     __BP_STMT_CASE82(prefix) case 83: goto BP_STMT_LABEL_N(prefix, 83);
#define __BP_STMT_CASE84(prefix)     __BP_STMT_CASE83(prefix) case 84: goto BP_STMT_LABEL_N(prefix, 84);
#define __BP_STMT_CASE85(prefix)     __BP_STMT_CASE84(prefix) case 85: goto BP_STMT_LABEL_N(prefix, 85);
#define __BP_STMT_CASE86(prefix)     __BP_STMT_CASE85(prefix) case 86: goto BP_STMT_LABEL_N(prefix, 86);
#define __BP_STMT_CASE87(prefix)     __BP_STMT_CASE86(prefix) case 87: goto BP_STMT_LABEL_N(prefix, 87);
#define __BP_STMT_CASE88(prefix)     __BP_STMT_CASE87(prefix) case 88: goto BP_STMT_LABEL_N(prefix, 88);
#define __BP_STMT_CASE89(prefix)     __BP_STMT_CASE88(prefix) case 89: goto BP_STMT_LABEL_N(prefix, 89);
#define __BP_STMT_CASE90(prefix)     __BP_STMT_CASE89(prefix) case 90: goto BP_STMT_LABEL_N(prefix, 90);
#define __BP_STMT_CASE91(prefix)     __BP_STMT_CASE90(prefix) case 91: goto BP_STMT_LABEL_N(prefix, 91);
#define __BP_STMT_CASE92(prefix)     __BP_STMT_CASE91(prefix) case 92: goto BP_STMT_LABEL_N(prefix, 92);
#define __BP_STMT_CASE93(prefix)     __BP_STMT_CASE92(prefix) case 93: goto BP_STMT_LABEL_N(prefix, 93);
#define __BP_STMT_CASE94(prefix)     __BP_STMT_CASE93(prefix) case 94: goto BP_STMT_LABEL_N(prefix, 94);
#define __BP_STMT_CASE95(prefix)     __BP_STMT_CASE94(prefix) case 95: goto BP_STMT_LABEL_N(prefix, 95);
#define __BP_STMT_CASE96(prefix)     __BP_STMT_CASE95(prefix) case 96: goto BP_STMT_LABEL_N(prefix, 96);
#define __BP_STMT_CASE97(prefix)     __BP_STMT_CASE96(prefix) case 97: goto BP_STMT_LABEL_N(prefix, 97);
#define __BP_STMT_CASE98(prefix)     __BP_STMT_CASE97(prefix) case 98: goto BP_STMT_LABEL_N(prefix, 98);
#define __BP_STMT_CASE99(prefix)     __BP_STMT_CASE98(prefix) case 99: goto BP_STMT_LABEL_N(prefix, 99);
#define __BP_STMT_CASE100(prefix)    __BP_STMT_CASE99(prefix) case 100: goto BP_STMT_LABEL_N(prefix, 100);
#define __BP_STMT_CASE101(prefix)    __BP_STMT_CASE100(prefix) case 101: goto BP_STMT_LABEL_N(prefix, 101);
#define __BP_STMT_CASE102(prefix)    __BP_STMT_CASE101(prefix) case 102: goto BP_STMT_LABEL_N(prefix, 102);
#define __BP_STMT_CASE103(prefix)    __BP_STMT_CASE102(prefix) case 103: goto BP_STMT_LABEL_N(prefix, 103);
#define __BP_STMT_CASE104(prefix)    __BP_STMT_CASE103(prefix) case 104: goto BP_STMT_LABEL_N(prefix, 104);
#define __BP_STMT_CASE105(prefix)    __BP_STMT_CASE104(prefix) case 105: goto BP_STMT_LABEL_N(prefix, 105);
#define __BP_STMT_CASE106(prefix)    __BP_STMT_CASE105(prefix) case 106: goto BP_STMT_LABEL_N(prefix, 106);
#define __BP_STMT_CASE107(prefix)    __BP_STMT_CASE106(prefix) case 107: goto BP_STMT_LABEL_N(prefix, 107);
#define __BP_STMT_CASE108(prefix)    __BP_STMT_CASE107(prefix) case 108: goto BP_STMT_LABEL_N(prefix, 108);
#define __BP_STMT_CASE109(prefix)    __BP_STMT_CASE108(prefix) case 109: goto BP_STMT_LABEL_N(prefix, 109);
#define __BP_STMT_CASE110(prefix)    __BP_STMT_CASE109(prefix) case 110: goto BP_STMT_LABEL_N(prefix, 110);
#define __BP_STMT_CASE111(prefix)    __BP_STMT_CASE110(prefix) case 111: goto BP_STMT_LABEL_N(prefix, 111);
#define __BP_STMT_CASE112(prefix)    __BP_STMT_CASE111(prefix) case 112: goto BP_STMT_LABEL_N(prefix, 112);
#define __BP_STMT_CASE113(prefix)    __BP_STMT_CASE112(prefix) case 113: goto BP_STMT_LABEL_N(prefix, 113);
#define __BP_STMT_CASE114(prefix)    __BP_STMT_CASE113(prefix) case 114: goto BP_STMT_LABEL_N(prefix, 114);
#define __BP_STMT_CASE115(prefix)    __BP_STMT_CASE114(prefix) case 115: goto BP_STMT_LABEL_N(prefix, 115);
#define __BP_STMT_CASE116(prefix)    __BP_STMT_CASE115(prefix) case 116: goto BP_STMT_LABEL_N(prefix, 116);
#define __BP_STMT_CASE117(prefix)    __BP_STMT_CASE116(prefix) case 117: goto BP_STMT_LABEL_N(prefix, 117);
#define __BP_STMT_CASE118(prefix)    __BP_STMT_CASE117(prefix) case 118: goto BP_STMT_LABEL_N(prefix, 118);
#define __BP_STMT_CASE119(prefix)    __BP_STMT_CASE118(prefix) case 119: goto BP_STMT_LABEL_N(prefix, 119);
#define __BP_STMT_CASE120(prefix)    __BP_STMT_CASE119(prefix) case 120: goto BP_STMT_LABEL_N(prefix, 120);
#define __BP_STMT_CASE121(prefix)    __BP_STMT_CASE120(prefix) case 121: goto BP_STMT_LABEL_N(prefix, 121);
#define __BP_STMT_CASE122(prefix)    __BP_STMT_CASE121(prefix) case 122: goto BP_STMT_LABEL_N(prefix, 122);
#define __BP_STMT_CASE123(prefix)    __BP_STMT_CASE122(prefix) case 123: goto BP_STMT_LABEL_N(prefix, 123);
#define __BP_STMT_CASE124(prefix)    __BP_STMT_CASE123(prefix) case 124: goto BP_STMT_LABEL_N(prefix, 124);
#define __BP_STMT_CASE125(prefix)    __BP_STMT_CASE124(prefix) case 125: goto BP_STMT_LABEL_N(prefix, 125);
#define __BP_STMT_CASE126(prefix)    __BP_STMT_CASE125(prefix) case 126: goto BP_STMT_LABEL_N(prefix, 126);
#define __BP_STMT_CASE127(prefix)    __BP_STMT_CASE126(prefix) case 127: goto BP_STMT_LABEL_N(prefix, 127);
#define __BP_STMT_CASE128(prefix)    __BP_STMT_CASE127(prefix) case 128: goto BP_STMT_LABEL_N(prefix, 128);
#define __BP_STMT_CASE129(prefix)    __BP_STMT_CASE128(prefix) case 129: goto BP_STMT_LABEL_N(prefix, 129);
#define __BP_STMT_CASE130(prefix)    __BP_STMT_CASE129(prefix) case 130: goto BP_STMT_LABEL_N(prefix, 130);
#define __BP_STMT_CASE131(prefix)    __BP_STMT_CASE130(prefix) case 131: goto BP_STMT_LABEL_N(prefix, 131);
#define __BP_STMT_CASE132(prefix)    __BP_STMT_CASE131(prefix) case 132: goto BP_STMT_LABEL_N(prefix, 132);
#define __BP_STMT_CASE133(prefix)    __BP_STMT_CASE132(prefix) case 133: goto BP_STMT_LABEL_N(prefix, 133);
#define __BP_STMT_CASE134(prefix)    __BP_STMT_CASE133(prefix) case 134: goto BP_STMT_LABEL_N(prefix, 134);
#define __BP_STMT_CASE135(prefix)    __BP_STMT_CASE134(prefix) case 135: goto BP_STMT_LABEL_N(prefix, 135);
#define __BP_STMT_CASE136(prefix)    __BP_STMT_CASE135(prefix) case 136: goto BP_STMT_LABEL_N(prefix, 136);
#define __BP_STMT_CASE137(prefix)    __BP_STMT_CASE136(prefix) case 137: goto BP_STMT_LABEL_N(prefix, 137);
#define __BP_STMT_CASE138(prefix)    __BP_STMT_CASE137(prefix) case 138: goto BP_STMT_LABEL_N(prefix, 138);
#define __BP_STMT_CASE139(prefix)    __BP_STMT_CASE138(prefix) case 139: goto BP_STMT_LABEL_N(prefix, 139);
#define __BP_STMT_CASE140(prefix)    __BP_STMT_CASE139(prefix) case 140: goto BP_STMT_LABEL_N(prefix, 140);
#define __BP_STMT_CASE141(prefix)    __BP_STMT_CASE140(prefix) case 141: goto BP_STMT_LABEL_N(prefix, 141);
#define __BP_STMT_CASE142(prefix)    __BP_STMT_CASE141(prefix) case 142: goto BP_STMT_LABEL_N(prefix, 142);
#define __BP_STMT_CASE143(prefix)    __BP_STMT_CASE142(prefix) case 143: goto BP_STMT_LABEL_N(prefix, 143);
#define __BP_STMT_CASE144(prefix)    __BP_STMT_CASE143(prefix) case 144: goto BP_STMT_LABEL_N(prefix, 144);
#define __BP_STMT_CASE145(prefix)    __BP_STMT_CASE144(prefix) case 145: goto BP_STMT_LABEL_N(prefix, 145);
#define __BP_STMT_CASE146(prefix)    __BP_STMT_CASE145(prefix) case 146: goto BP_STMT_LABEL_N(prefix, 146);
#define __BP_STMT_CASE147(prefix)    __BP_STMT_CASE146(prefix) case 147: goto BP_STMT_LABEL_N(prefix, 147);
#define __BP_STMT_CASE148(prefix)    __BP_STMT_CASE147(prefix) case 148: goto BP_STMT_LABEL_N(prefix, 148);
#define __BP_STMT_CASE149(prefix)    __BP_STMT_CASE148(prefix) case 149: goto BP_STMT_LABEL_N(prefix, 149);
#define __BP_STMT_CASE150(prefix)    __BP_STMT_CASE149(prefix) case 150: goto BP_STMT_LABEL_N(prefix, 150);
#define __BP_STMT_CASE151(prefix)    __BP_STMT_CASE150(prefix) case 151: goto BP_STMT_LABEL_N(prefix, 151);
#define __BP_STMT_CASE152(prefix)    __BP_STMT_CASE151(prefix) case 152: goto BP_STMT_LABEL_N(prefix, 152);
#define __BP_STMT_CASE153(prefix)    __BP_STMT_CASE152(prefix) case 153: goto BP_STMT_LABEL_N(prefix, 153);
#define __BP_STMT_CASE154(prefix)    __BP_STMT_CASE153(prefix) case 154: goto BP_STMT_LABEL_N(prefix, 154);
#define __BP_STMT_CASE155(prefix)    __BP_STMT_CASE154(prefix) case 155: goto BP_STMT_LABEL_N(prefix, 155);
#define __BP_STMT_CASE156(prefix)    __BP_STMT_CASE155(prefix) case 156: goto BP_STMT_LABEL_N(prefix, 156);
#define __BP_STMT_CASE157(prefix)    __BP_STMT_CASE156(prefix) case 157: goto BP_STMT_LABEL_N(prefix, 157);
#define __BP_STMT_CASE158(prefix)    __BP_STMT_CASE157(prefix) case 158: goto BP_STMT_LABEL_N(prefix, 158);
#define __BP_STMT_CASE159(prefix)    __BP_STMT_CASE158(prefix) case 159: goto BP_STMT_LABEL_N(prefix, 159);
#define __BP_STMT_CASE160(prefix)    __BP_STMT_CASE159(prefix) case 160: goto BP_STMT_LABEL_N(prefix, 160);
#define __BP_STMT_CASE161(prefix)    __BP_STMT_CASE160(prefix) case 161: goto BP_STMT_LABEL_N(prefix, 161);
#define __BP_STMT_CASE162(prefix)    __BP_STMT_CASE161(prefix) case 162: goto BP_STMT_LABEL_N(prefix, 162);
#define __BP_STMT_CASE163(prefix)    __BP_STMT_CASE162(prefix) case 163: goto BP_STMT_LABEL_N(prefix, 163);
#define __BP_STMT_CASE164(prefix)    __BP_STMT_CASE163(prefix) case 164: goto BP_STMT_LABEL_N(prefix, 164);
#define __BP_STMT_CASE165(prefix)    __BP_STMT_CASE164(prefix) case 165: goto BP_STMT_LABEL_N(prefix, 165);
#define __BP_STMT_CASE166(prefix)    __BP_STMT_CASE165(prefix) case 166: goto BP_STMT_LABEL_N(prefix, 166);
#define __BP_STMT_CASE167(prefix)    __BP_STMT_CASE166(prefix) case 167: goto BP_STMT_LABEL_N(prefix, 167);
#define __BP_STMT_CASE168(prefix)    __BP_STMT_CASE167(prefix) case 168: goto BP_STMT_LABEL_N(prefix, 168);
#define __BP_STMT_CASE169(prefix)    __BP_STMT_CASE168(prefix) case 169: goto BP_STMT_LABEL_N(prefix, 169);
#define __BP_STMT_CASE170(prefix)    __BP_STMT_CASE169(prefix) case 170: goto BP_STMT_LABEL_N(prefix, 170);
#define __BP_STMT_CASE171(prefix)    __BP_STMT_CASE170(prefix) case 171: goto BP_STMT_LABEL_N(prefix, 171);
#define __BP_STMT_CASE172(prefix)    __BP_STMT_CASE171(prefix) case 172: goto BP_STMT_LABEL_N(prefix, 172);
#define __BP_STMT_CASE173(prefix)    __BP_STMT_CASE172(prefix) case 173: goto BP_STMT_LABEL_N(prefix, 173);
#define __BP_STMT_CASE174(prefix)    __BP_STMT_CASE173(prefix) case 174: goto BP_STMT_LABEL_N(prefix, 174);
#define __BP_STMT_CASE175(prefix)    __BP_STMT_CASE174(prefix) case 175: goto BP_STMT_LABEL_N(prefix, 175);
#define __BP_STMT_CASE176(prefix)    __BP_STMT_CASE175(prefix) case 176: goto BP_STMT_LABEL_N(prefix, 176);
#define __BP_STMT_CASE177(prefix)    __BP_STMT_CASE176(prefix) case 177: goto BP_STMT_LABEL_N(prefix, 177);
#define __BP_STMT_CASE178(prefix)    __BP_STMT_CASE177(prefix) case 178: goto BP_STMT_LABEL_N(prefix, 178);
#define __BP_STMT_CASE179(prefix)    __BP_STMT_CASE178(prefix) case 179: goto BP_STMT_LABEL_N(prefix, 179);
#define __BP_STMT_CASE180(prefix)    __BP_STMT_CASE179(prefix) case 180: goto BP_STMT_LABEL_N(prefix, 180);
#define __BP_STMT_CASE181(prefix)    __BP_STMT_CASE180(prefix) case 181: goto BP_STMT_LABEL_N(prefix, 181);
#define __BP_STMT_CASE182(prefix)    __BP_STMT_CASE181(prefix) case 182: goto BP_STMT_LABEL_N(prefix, 182);
#define __BP_STMT_CASE183(prefix)    __BP_STMT_CASE182(prefix) case 183: goto BP_STMT_LABEL_N(prefix, 183);
#define __BP_STMT_CASE184(prefix)    __BP_STMT_CASE183(prefix) case 184: goto BP_STMT_LABEL_N(prefix, 184);
#define __BP_STMT_CASE185(prefix)    __BP_STMT_CASE184(prefix) case 185: goto BP_STMT_LABEL_N(prefix, 185);
#define __BP_STMT_CASE186(prefix)    __BP_STMT_CASE185(prefix) case 186: goto BP_STMT_LABEL_N(prefix, 186);
#define __BP_STMT_CASE187(prefix)    __BP_STMT_CASE186(prefix) case 187: goto BP_STMT_LABEL_N(prefix, 187);
#define __BP_STMT_CASE188(prefix)    __BP_STMT_CASE187(prefix) case 188: goto BP_STMT_LABEL_N(prefix, 188);
#define __BP_STMT_CASE189(prefix)    __BP_STMT_CASE188(prefix) case 189: goto BP_STMT_LABEL_N(prefix, 189);
#define __BP_STMT_CASE190(prefix)    __BP_STMT_CASE189(prefix) case 190: goto BP_STMT_LABEL_N(prefix, 190);
#define __BP_STMT_CASE191(prefix)    __BP_STMT_CASE190(prefix) case 191: goto BP_STMT_LABEL_N(prefix, 191);
#define __BP_STMT_CASE192(prefix)    __BP_STMT_CASE191(prefix) case 192: goto BP_STMT_LABEL_N(prefix, 192);
#define __BP_STMT_CASE193(prefix)    __BP_STMT_CASE192(prefix) case 193: goto BP_STMT_LABEL_N(prefix, 193);
#define __BP_STMT_CASE194(prefix)    __BP_STMT_CASE193(prefix) case 194: goto BP_STMT_LABEL_N(prefix, 194);
#define __BP_STMT_CASE195(prefix)    __BP_STMT_CASE194(prefix) case 195: goto BP_STMT_LABEL_N(prefix, 195);
#define __BP_STMT_CASE196(prefix)    __BP_STMT_CASE195(prefix) case 196: goto BP_STMT_LABEL_N(prefix, 196);
#define __BP_STMT_CASE197(prefix)    __BP_STMT_CASE196(prefix) case 197: goto BP_STMT_LABEL_N(prefix, 197);
#define __BP_STMT_CASE198(prefix)    __BP_STMT_CASE197(prefix) case 198: goto BP_STMT_LABEL_N(prefix, 198);
#define __BP_STMT_CASE199(prefix)    __BP_STMT_CASE198(prefix) case 199: goto BP_STMT_LABEL_N(prefix, 199);
#define __BP_STMT_CASE200(prefix)    __BP_STMT_CASE199(prefix) case 200: goto BP_STMT_LABEL_N(prefix, 200);
#define __BP_STMT_CASE201(prefix)    __BP_STMT_CASE200(prefix) case 201: goto BP_STMT_LABEL_N(prefix, 201);
#define __BP_STMT_CASE202(prefix)    __BP_STMT_CASE201(prefix) case 202: goto BP_STMT_LABEL_N(prefix, 202);
#define __BP_STMT_CASE203(prefix)    __BP_STMT_CASE202(prefix) case 203: goto BP_STMT_LABEL_N(prefix, 203);
#define __BP_STMT_CASE204(prefix)    __BP_STMT_CASE203(prefix) case 204: goto BP_STMT_LABEL_N(prefix, 204);
#define __BP_STMT_CASE205(prefix)    __BP_STMT_CASE204(prefix) case 205: goto BP_STMT_LABEL_N(prefix, 205);
#define __BP_STMT_CASE206(prefix)    __BP_STMT_CASE205(prefix) case 206: goto BP_STMT_LABEL_N(prefix, 206);
#define __BP_STMT_CASE207(prefix)    __BP_STMT_CASE206(prefix) case 207: goto BP_STMT_LABEL_N(prefix, 207);
#define __BP_STMT_CASE208(prefix)    __BP_STMT_CASE207(prefix) case 208: goto BP_STMT_LABEL_N(prefix, 208);
#define __BP_STMT_CASE209(prefix)    __BP_STMT_CASE208(prefix) case 209: goto BP_STMT_LABEL_N(prefix, 209);
#define __BP_STMT_CASE210(prefix)    __BP_STMT_CASE209(prefix) case 210: goto BP_STMT_LABEL_N(prefix, 210);
#define __BP_STMT_CASE211(prefix)    __BP_STMT_CASE210(prefix) case 211: goto BP_STMT_LABEL_N(prefix, 211);
#define __BP_STMT_CASE212(prefix)    __BP_STMT_CASE211(prefix) case 212: goto BP_STMT_LABEL_N(prefix, 212);
#define __BP_STMT_CASE213(prefix)    __BP_STMT_CASE212(prefix) case 213: goto BP_STMT_LABEL_N(prefix, 213);
#define __BP_STMT_CASE214(prefix)    __BP_STMT_CASE213(prefix) case 214: goto BP_STMT_LABEL_N(prefix, 214);
#define __BP_STMT_CASE215(prefix)    __BP_STMT_CASE214(prefix) case 215: goto BP_STMT_LABEL_N(prefix, 215);
#define __BP_STMT_CASE216(prefix)    __BP_STMT_CASE215(prefix) case 216: goto BP_STMT_LABEL_N(prefix, 216);
#define __BP_STMT_CASE217(prefix)    __BP_STMT_CASE216(prefix) case 217: goto BP_STMT_LABEL_N(prefix, 217);
#define __BP_STMT_CASE218(prefix)    __BP_STMT_CASE217(prefix) case 218: goto BP_STMT_LABEL_N(prefix, 218);
#define __BP_STMT_CASE219(prefix)    __BP_STMT_CASE218(prefix) case 219: goto BP_STMT_LABEL_N(prefix, 219);
#define __BP_STMT_CASE220(prefix)    __BP_STMT_CASE219(prefix) case 220: goto BP_STMT_LABEL_N(prefix, 220);
#define __BP_STMT_CASE221(prefix)    __BP_STMT_CASE220(prefix) case 221: goto BP_STMT_LABEL_N(prefix, 221);
#define __BP_STMT_CASE222(prefix)    __BP_STMT_CASE221(prefix) case 222: goto BP_STMT_LABEL_N(prefix, 222);
#define __BP_STMT_CASE223(prefix)    __BP_STMT_CASE222(prefix) case 223: goto BP_STMT_LABEL_N(prefix, 223);
#define __BP_STMT_CASE224(prefix)    __BP_STMT_CASE223(prefix) case 224: goto BP_STMT_LABEL_N(prefix, 224);
#define __BP_STMT_CASE225(prefix)    __BP_STMT_CASE224(prefix) case 225: goto BP_STMT_LABEL_N(prefix, 225);
#define __BP_STMT_CASE226(prefix)    __BP_STMT_CASE225(prefix) case 226: goto BP_STMT_LABEL_N(prefix, 226);
#define __BP_STMT_CASE227(prefix)    __BP_STMT_CASE226(prefix) case 227: goto BP_STMT_LABEL_N(prefix, 227);
#define __BP_STMT_CASE228(prefix)    __BP_STMT_CASE227(prefix) case 228: goto BP_STMT_LABEL_N(prefix, 228);
#define __BP_STMT_CASE229(prefix)    __BP_STMT_CASE228(prefix) case 229: goto BP_STMT_LABEL_N(prefix, 229);
#define __BP_STMT_CASE230(prefix)    __BP_STMT_CASE229(prefix) case 230: goto BP_STMT_LABEL_N(prefix, 230);
#define __BP_STMT_CASE231(prefix)    __BP_STMT_CASE230(prefix) case 231: goto BP_STMT_LABEL_N(prefix, 231);
#define __BP_STMT_CASE232(prefix)    __BP_STMT_CASE231(prefix) case 232: goto BP_STMT_LABEL_N(prefix, 232);
#define __BP_STMT_CASE233(prefix)    __BP_STMT_CASE232(prefix) case 233: goto BP_STMT_LABEL_N(prefix, 233);
#define __BP_STMT_CASE234(prefix)    __BP_STMT_CASE233(prefix) case 234: goto BP_STMT_LABEL_N(prefix, 234);
#define __BP_STMT_CASE235(prefix)    __BP_STMT_CASE234(prefix) case 235: goto BP_STMT_LABEL_N(prefix, 235);
#define __BP_STMT_CASE236(prefix)    __BP_STMT_CASE235(prefix) case 236: goto BP_STMT_LABEL_N(prefix, 236);
#define __BP_STMT_CASE237(prefix)    __BP_STMT_CASE236(prefix) case 237: goto BP_STMT_LABEL_N(prefix, 237);
#define __BP_STMT_CASE238(prefix)    __BP_STMT_CASE237(prefix) case 238: goto BP_STMT_LABEL_N(prefix, 238);
#define __BP_STMT_CASE239(prefix)    __BP_STMT_CASE238(prefix) case 239: goto BP_STMT_LABEL_N(prefix, 239);
#define __BP_STMT_CASE240(prefix)    __BP_STMT_CASE239(prefix) case 240: goto BP_STMT_LABEL_N(prefix, 240);
#define __BP_STMT_CASE241(prefix)    __BP_STMT_CASE240(prefix) case 241: goto BP_STMT_LABEL_N(prefix, 241);
#define __BP_STMT_CASE242(prefix)    __BP_STMT_CASE241(prefix) case 242: goto BP_STMT_LABEL_N(prefix, 242);
#define __BP_STMT_CASE243(prefix)    __BP_STMT_CASE242(prefix) case 243: goto BP_STMT_LABEL_N(prefix, 243);
#define __BP_STMT_CASE244(prefix)    __BP_STMT_CASE243(prefix) case 244: goto BP_STMT_LABEL_N(prefix, 244);
#define __BP_STMT_CASE245(prefix)    __BP_STMT_CASE244(prefix) case 245: goto BP_STMT_LABEL_N(prefix, 245);
#define __BP_STMT_CASE246(prefix)    __BP_STMT_CASE245(prefix) case 246: goto BP_STMT_LABEL_N(prefix, 246);
#define __BP_STMT_CASE247(prefix)    __BP_STMT_CASE246(prefix) case 247: goto BP_STMT_LABEL_N(prefix, 247);
#define __BP_STMT_CASE248(prefix)    __BP_STMT_CASE247(prefix) case 248: goto BP_STMT_LABEL_N(prefix, 248);
#define __BP_STMT_CASE249(prefix)    __BP_STMT_CASE248(prefix) case 249: goto BP_STMT_LABEL_N(prefix, 249);
#define __BP_STMT_CASE250(prefix)    __BP_STMT_CASE249(prefix) case 250: goto BP_STMT_LABEL_N(prefix, 250);
#define __BP_STMT_CASE251(prefix)    __BP_STMT_CASE250(prefix) case 251: goto BP_STMT_LABEL_N(prefix, 251);
#define __BP_STMT_CASE252(prefix)    __BP_STMT_CASE251(prefix) case 252: goto BP_STMT_LABEL_N(prefix, 252);
#define __BP_STMT_CASE253(prefix)    __BP_STMT_CASE252(prefix) case 253: goto BP_STMT_LABEL_N(prefix, 253);
#define __BP_STMT_CASE254(prefix)    __BP_STMT_CASE253(prefix) case 254: goto BP_STMT_LABEL_N(prefix, 254);
#define __BP_STMT_CASE255(prefix)    __BP_STMT_CASE254(prefix) case 255: goto BP_STMT_LABEL_N(prefix, 255);

/* then define a __BP_STMT_CAT_connection macro for use by __BP_STMT_CASEN */
/* 再定义一个__BP_STMT_CAT_连接宏，供__BP_STMT_CASEN使用 */
#define __BP_STMT_CAT_(x, y)     __BP_STMT_CAT_1(x, y)
#define __BP_STMT_CAT_1(x, y)    x##y

/* because __BP_STMT_CASE contains the __BP_STMT_CAT macro,
 * you cannot use the __BP_STMT_CAT macro again,
 * so use the __BP_STMT_CAT_ macro */
/* 由于__BP_STMT_CASE中包含了__BP_STMT_CAT宏，
 * 因此不可再次使用__BP_STMT_CAT宏，
 * 故使用__BP_STMT_CAT_宏 */
#define __BP_STMT_CASEN(bp_nums) __BP_STMT_CAT_(__BP_STMT_CASE, bp_nums)


/*********************************************************
 *@brief: 
 ***start a bp_stmt coroutine
 *
 *@contract: 
 ***1. "bp_nums" must be a pure number, or a macro that is expanded to be a pure number
 ***2. "bp_nums" indicates the number of breakpoints contained between bp_stmt_begin() and bp_stmt_end()
 ***3. the breakpoint number between bp_stmt_begin() and bp_stmt_end() is continuous from 1 to n,
 ***   and must not be missed or redundant.
 *
 *@parameter:
 *[prefix]: the prefix of this statement block
 *[bp_nums]: indicates the number of breakpoints contained between bp_stmt_begin() and bp_stmt_end()
 *[bp_stmt]: variable(1 byte) for recording the current breakpoint position
 *********************************************************/
/*********************************************************
 *@简要：
 ***开始一个bp_stmt协程
 *
 *@约定：
 ***1、bp_nums必须是纯数字，或者展开后为纯数字的宏
 ***2、bp_nums表示从bp_stmt_begin()到bp_stmt_end()之间包含的断点个数
 ***3、bp_stmt_begin()到bp_stmt_end()之间的断点号从1到n连续，不可遗漏与多余
 *
 *@参数：
 *[prfix]: 该语句块的前缀
 *[bp_nums]：表示bp_stmt_begin()到bp_stmt_end()之间包含的断点个数
 *[bp]：用于记录当前断点位置的变量（一个字节）
 **********************************************************/
#define bp_stmt_begin(prefix, bp_nums, bp)   __BP_STMT_HEADER(bp) __BP_STMT_CASEN(bp_nums)(prefix) __BP_STMT_TAIL(prefix)


/*********************************************************
 *@brief: 
 ***together with bp_stmt_begin(), constitutes the scope of the coroutine
 *
 *@parameter:
 *[prefix]: the prefix of this statement block
 *[bp_stmt]: variable(1 byte) for recording the current breakpoint position, the life cycle is global
 *********************************************************/
/*********************************************************
 *@简要：
 ***与bp_stmt_begin()组成协程的作用域
 *
 *@参数：
 *[prfix]: 该语句块的前缀
 *[bp_stmt]：用于记录当前断点位置的生命周期为全局的变量（一个字节）
 **********************************************************/
#define bp_stmt_end(prefix, bp)  \
    do { BP_STMT_LABEL_END(prefix):; (bp) = BP_STMT_INIT_VAL; goto __BP_STMT_CAT_(BP_STMT_LABEL_END(prefix), 1); __BP_STMT_CAT_(BP_STMT_LABEL_END(prefix), 1):; } while (0)


/************************************************************
 *@brief:
 ***record the breakpoint number in the bp_stmt variable
 *
 *@parameter:
 *[bp_stmt_num]: recorded breakpoint number
 *[bp_stmt]: variable(1 byte) for recording the current breakpoint position
 *************************************************************/
/************************************************************
 *@简介：
 ***将断点号记录在bp_stmt变量之中
 *
 *@参数：
 *[bp_stmt_num]：记录的断点号
 *[bp_stmt]：用于记录当前断点位置的变量（一个字节）
 *************************************************************/
#define bp_stmt_set(bp_num, bp)   ((int)((bp) = (bp_num)))


/************************************************************
 *@brief:
 ***create a recovery location with a breakpoint of bp_stmt_num,
 ***and bp_stmt_begin() will jump to the appropriate recovery location
 ***based on the breakpoint number in the bp_stmt variable.
 *
 *@contract:
 ***1. create a recovery location with a different bp_stmt_num each time
 ***2. the breakpoint number between bp_stmt_begin() and bp_stmt_end() is continuous from 1 to n,
 ***   and must not be missed or redundant.
 ***3. "bp_nums" must be a pure number, or a macro that is expanded to be a pure number
 * 
 *@parameter:
 *[prefix]: the prefix of this statement block
 *[bp_num]: corresponding breakpoint number
 *************************************************************/
/************************************************************
 *@简介：
 ***创建一个断点号为bp_stmt_num的恢复位置，bp_stmt_begin()会根据bp_stmt变量中的断点号
 ***跳转到相应的恢复位置。
 *
 *@约定：
 ***1、每次都使用不同的bp_stmt_num创建恢复位置
 ***2、bp_stmt_begin()到bp_stmt_end()之间的断点号从1到n连续，不可遗漏与多余
 ***3、bp_nums必须是纯数字，或者展开后为纯数字的宏
 *
 *@参数：
 *[prefix]: 该语句块的前缀
 *[bp_num]：对应的断点号
 *************************************************************/
#define bp_stmt_restore_point(prefix, bp_num)        BP_STMT_LABEL_N(prefix, bp_num)


/*************************************************************************
 * BP_STMT initialization
 * BP_STMT初始化
 *************************************************************************
 */
#define BP_STMT_INIT_VAL     0
#define bp_stmt_init(bp)     ((bp) = BP_STMT_INIT_VAL)


/*********************************************************
 *@description: 
 ***BP_STMT yield series
 *
 *@note:
 ***the bp_stmt_yield family of macros consists of bp_stmt_set and bp_stmt_restore_point,
 ***so you need to follow the usage contract of bp_stmt_restore_point.
 ***as follows:
 ***1. "bp_nums" must be a pure number, or a macro that is expanded to be a pure number
 ***2. the breakpoint number between bp_stmt_begin() and bp_stmt_end() is continuous from 1 to n,
 ***   and must not be missed or redundant.
 *********************************************************
 *@说明：
 ***BP_STMT yield系列宏
 *
 *@注意：
 ***bp_stmt_yield系列宏由bp_stmt_set与bp_stmt_restore_point组成，因此需要
 ***遵守bp_stmt_restore_point的使用约定。
 ***如下：
 ***1、bp_nums必须是纯数字，或者展开后为纯数字的宏
 ***2、bp_stmt_begin()到bp_stmt_end()之间的断点号从1到n连续，不可遗漏与多余
 *********************************************************/

/* yield, restore the current execution position in the next enter */
/* yield，并在下次进入恢复当前执行位置 */
#define bp_stmt_yield(prefix, bp_num, bp)           \
    if (0)                                          \
    {                                               \
        bp_stmt_restore_point(prefix, bp_num):;     \
    }                                               \
    else while (bp_stmt_set(bp_num, bp))            \
        goto __BP_STMT_CAT_(BP_STMT_LABEL_END(prefix), 1);

#define bp_stmt_yield_until(prefix, bp_num, bp, cond)   \
    do {                                                \
        while (!(cond))                                 \
        {                                               \
            bp_stmt_yield(prefix, bp_num, (bp));        \
        }                                               \
    } while (0)

#define bp_stmt_yield_while(prefix, bp_num, bp, cond)   \
    do {                                                \
        while (cond)                                    \
        {                                               \
            bp_stmt_yield(prefix, bp_num, (bp));        \
        }                                               \
    } while (0)

/*************************************************************************
 * BP_STMT other series
 * BP_STMT 其他系列
 *************************************************************************
 */

/* jump out of the BP_STMT_BEGIN to BP_STMT_END statement */
/* 跳出BP_STMT_BEGIN到BP_STMT_END包含的语句之外 */
#define bp_stmt_break(prefix)    \
    do {goto BP_STMT_LABEL_END(prefix); } while (0)

/* re-execution after BP_STMT_BEGIN */
/* 从BP_STMT_BEGIN之后重新执行 */
#define bp_stmt_continue(prefix) \
    do { goto BP_STMT_LABEL_START(prefix); } while (0)

/* exit BP_STMT_BEGIN-BP_STMT_END statement block,
 * the next call will be re-executed */
/* 退出BP_STMT_BEGIN-BP_STMT_END语句块，下次进入将重新执行 */
#define bp_stmt_exit bp_stmt_break

/************************************************************
 * define a bpd_stmt pointer to use BP_STMTD series macros,
 * omit bp_stmt parameter
 * usage：
 *
 *  #define BP_STMT_DEFAULT_PREFIX demo
 *  #define BP_STMT_DEFAULT vars->bp1
 *  bpd_stmt_begin(1)
 *
 *  bpd_stmt_begin(1)
 *  {
 *      bpd_stmt_yield(1);
 *  }
 *  bpd_stmt_end();
 *  #undef BP_STMT_DEFAULT
 *  #undef BP_STMT_DEFAULT_PREFIX
 *
 ************************************************************/
/************************************************************
 * 定义一个BP_STMT_DEFAULT与BP_STMT_DEFAULT_PREFIX宏，可使用bpd_stmt系列宏，省略bp参数
 * 例：
 *
 *  #define BP_STMT_DEFAULT_PREFIX demo
 *  #define BP_STMT_DEFAULT vars->bp1
 *  bpd_stmt_begin(1)
 *  {
 *      bpd_stmt_yield(1);
 *  }
 *  bpd_stmt_end();
 *  #undef BP_STMT_DEFAULT
 *  #undef BP_STMT_DEFAULT_PREFIX
 *
 ************************************************************/

/* default bpd_stmt as the bp_stmt parameter
 * of the bp_stmt series macro */
/* 将bpd_stmt默认作为bp_stmt系列宏的bp_stmt参数 */

#define BPD_STMT_INIT_VAL                                0

#define bpd_stmt_init(bp)                                ((BP_STMT_DEFAULT) = BP_STMT_INIT_VAL)

#define bpd_stmt_restore_point(point)                    bp_stmt_restore_point(BP_STMT_DEFAULT_PREFIX, point)

#define bpd_stmt_begin(bp_nums)                          bp_stmt_begin(BP_STMT_DEFAULT_PREFIX, bp_nums, (BP_STMT_DEFAULT))

#define bpd_stmt_end()                                   bp_stmt_end(BP_STMT_DEFAULT_PREFIX, (BP_STMT_DEFAULT))

#define bpd_stmt_set(bp_nums)                            bp_stmt_set(bp_nums, (BP_STMT_DEFAULT))

#define bpd_stmt_yield(bp_nums)                          bp_stmt_yield(BP_STMT_DEFAULT_PREFIX, bp_nums, (BP_STMT_DEFAULT))

#define bpd_stmt_yield_until(bp_nums, cond)              bp_stmt_yield_until(BP_STMT_DEFAULT_PREFIX, bp_nums, (BP_STMT_DEFAULT), (cond))

#define bpd_stmt_yield_while(bp_nums, cond)              bp_stmt_yield_while(BP_STMT_DEFAULT_PREFIX, bp_nums, (BP_STMT_DEFAULT), (cond))

#define bpd_stmt_break                                   bp_stmt_break(BP_STMT_DEFAULT_PREFIX)

#define bpd_stmt_continue                                bp_stmt_continue(BP_STMT_DEFAULT_PREFIX)

#define bpd_stmt_exit                                    bpd_stmt_break

#endif /* __BP_STMT_STMT_H__ */
