/*
 * Copyright (C) 2018 xiaoliang<1296283984@qq.com>.
 */

/************************************************************************
 * BP (breakpoint), a programming method that sets a breakpoint
 * in a function and resumes on the next call
 * BP(breakpoint)，一种可在函数中设置断点，且在下次调用时恢复的编程方式
 ************************************************************************
 */

/************************************************************************
 * BP series usage:
 *
 * //use static variables or external data structures to save function breakpoints
 * static uint8_t bp = BP_INIT_VAL;
 * static uint8_t *bpd = &bp;
 *
 * //intercept callback before bp_begin and check breakpoint status
 * if (bp == 1) //bp is currently at breakpoint 1
 * {
 *      //check the status and events
 *      if (event != timer1)
 *      {
 *          //Reject this event
 *          return ;
 *      }
 * }
 *
 * //start a coroutine with two breakpoints
 * bpd_begin(2)
 *
 * while (1)
 * {
 *      //set its own callback, trigger the next call
 *      timer_start(timer1, self_callback, 1000);
 *
 *      //active return, and restore the current position on the next callback
 *      bpd_yield(1);
 *
 *      //the timer arrives that turned on the led
 *      led_on();
 *
 *      //set its own callback, trigger the next call
 *      timer_start(timer2, self_callback, 1000);
 *
 *      //active return, and restore the current position on the next callback
 *      bpd_yield(2);
 *
 *      //the timer arrives that turned off the led
 *      led_off();
 *
 * }
 *
 * bp_end(bp);
 *
 ************************************************************************
 */

/************************************************************************
 * 用法说明：
 * BP系列用法：
 *
 * //使用静态变量或者外部数据结构保存函数的断点
 * static uint8_t bp = BP_INIT_VAL;
 * static uint8_t *bpd = &bp;
 *
 * //在bp_begin之前可以拦截回调，并检查断点状态
 * if (bp == 1) //bp当前处于断点1
 * {
 *    //对状态与事件进行检查
 *    if (event != timer1)
 *    {
 *       //拒绝该事件
 *       return ;
 *    }
 * }
 *
 * //开始包含2个断点的协程
 * bpd_begin(2)
 *
 * while (1)
 * {
 *      //设置自身回调，触发下次调用
 *      timer_start(timer1, self_callback, 1000);
 *
 *      //主动返回，并在下次回调时恢复当前位置
 *      bpd_yield(1);
 *
 *      //定时器到达，打开LED
 *      led_on();
 *
 *      //设置自身回调，触发下次调用
 *      timer_start(timer2, self_callback, 1000);
 *
 *      //主动返回，并在下次回调时恢复当前位置
 *      bpd_yield(2);
 *
 *      //定时器到达，关闭LED
 *      led_off();
 * }
 *
 * bp_end(bp);
 *
 ************************************************************************
 */

#ifndef __BP_H__
#define __BP_H__

/*************************************************************************
 * BP macro build
 * BP 宏构造
 *************************************************************************
 */

/* BP macro connection, and fully expand */
/* BP宏连接，并完全展开 */
#define __BP_CAT(x, y)  __BP_CAT1(x, y)
#define __BP_CAT1(x, y) x##y

/* BP label prefix definition */
/* BP标号前缀定义 */
#define BP_LABEL_PREFIX         label_bp_

/* BP label with N suffix */
/* 以N为后缀的BP标签 */
#define BP_LABEL_N(n)           __BP_CAT(BP_LABEL_PREFIX, n)

/* BP start label */
/* BP开始标签 */
#define BP_LABEL_START          BP_LABEL_N(0)

/* BP end label */
/* BP结束标签 */
#define BP_LABEL_END            BP_LABEL_N(end)

/* the head of the BP_BEGIN,
 * the switch-goto statement block */
/* BP_BEGIN的头部，switch-goto语句块 */
#define __BP_HEADER(bp)         do {                                            \
                                    switch ((bp))                               \
                                    {                                           \

/* the tail of the BP_BEGIN,
 * the switch-goto statement block */
/* BP_BEGIN的尾部，switch-goto语句块 */
#define __BP_TAIL                       default:                                \
                                        goto BP_LABEL_END;                      \
                                    }                                           \
                                    BP_LABEL_START:;                            \
                                } while (0)                                     \

/* the block part of the case statement for BP_BEGIN */
/* BP_BEGIN的case语句块部分 */
#define __BP_CASE0      case 0: goto BP_LABEL_START;
#define __BP_CASE1      __BP_CASE0 case 1: goto BP_LABEL_N(1);
#define __BP_CASE2      __BP_CASE1 case 2: goto BP_LABEL_N(2);
#define __BP_CASE3      __BP_CASE2 case 3: goto BP_LABEL_N(3);
#define __BP_CASE4      __BP_CASE3 case 4: goto BP_LABEL_N(4);
#define __BP_CASE5      __BP_CASE4 case 5: goto BP_LABEL_N(5);
#define __BP_CASE6      __BP_CASE5 case 6: goto BP_LABEL_N(6);
#define __BP_CASE7      __BP_CASE6 case 7: goto BP_LABEL_N(7);
#define __BP_CASE8      __BP_CASE7 case 8: goto BP_LABEL_N(8);
#define __BP_CASE9      __BP_CASE8 case 9: goto BP_LABEL_N(9);
#define __BP_CASE10     __BP_CASE9 case 10: goto BP_LABEL_N(10);
#define __BP_CASE11     __BP_CASE10 case 11: goto BP_LABEL_N(11);
#define __BP_CASE12     __BP_CASE11 case 12: goto BP_LABEL_N(12);
#define __BP_CASE13     __BP_CASE12 case 13: goto BP_LABEL_N(13);
#define __BP_CASE14     __BP_CASE13 case 14: goto BP_LABEL_N(14);
#define __BP_CASE15     __BP_CASE14 case 15: goto BP_LABEL_N(15);
#define __BP_CASE16     __BP_CASE15 case 16: goto BP_LABEL_N(16);
#define __BP_CASE17     __BP_CASE16 case 17: goto BP_LABEL_N(17);
#define __BP_CASE18     __BP_CASE17 case 18: goto BP_LABEL_N(18);
#define __BP_CASE19     __BP_CASE18 case 19: goto BP_LABEL_N(19);
#define __BP_CASE20     __BP_CASE19 case 20: goto BP_LABEL_N(20);
#define __BP_CASE21     __BP_CASE20 case 21: goto BP_LABEL_N(21);
#define __BP_CASE22     __BP_CASE21 case 22: goto BP_LABEL_N(22);
#define __BP_CASE23     __BP_CASE22 case 23: goto BP_LABEL_N(23);
#define __BP_CASE24     __BP_CASE23 case 24: goto BP_LABEL_N(24);
#define __BP_CASE25     __BP_CASE24 case 25: goto BP_LABEL_N(25);
#define __BP_CASE26     __BP_CASE25 case 26: goto BP_LABEL_N(26);
#define __BP_CASE27     __BP_CASE26 case 27: goto BP_LABEL_N(27);
#define __BP_CASE28     __BP_CASE27 case 28: goto BP_LABEL_N(28);
#define __BP_CASE29     __BP_CASE28 case 29: goto BP_LABEL_N(29);
#define __BP_CASE30     __BP_CASE29 case 30: goto BP_LABEL_N(30);
#define __BP_CASE31     __BP_CASE30 case 31: goto BP_LABEL_N(31);
#define __BP_CASE32     __BP_CASE31 case 32: goto BP_LABEL_N(32);
#define __BP_CASE33     __BP_CASE32 case 33: goto BP_LABEL_N(33);
#define __BP_CASE34     __BP_CASE33 case 34: goto BP_LABEL_N(34);
#define __BP_CASE35     __BP_CASE34 case 35: goto BP_LABEL_N(35);
#define __BP_CASE36     __BP_CASE35 case 36: goto BP_LABEL_N(36);
#define __BP_CASE37     __BP_CASE36 case 37: goto BP_LABEL_N(37);
#define __BP_CASE38     __BP_CASE37 case 38: goto BP_LABEL_N(38);
#define __BP_CASE39     __BP_CASE38 case 39: goto BP_LABEL_N(39);
#define __BP_CASE40     __BP_CASE39 case 40: goto BP_LABEL_N(40);
#define __BP_CASE41     __BP_CASE40 case 41: goto BP_LABEL_N(41);
#define __BP_CASE42     __BP_CASE41 case 42: goto BP_LABEL_N(42);
#define __BP_CASE43     __BP_CASE42 case 43: goto BP_LABEL_N(43);
#define __BP_CASE44     __BP_CASE43 case 44: goto BP_LABEL_N(44);
#define __BP_CASE45     __BP_CASE44 case 45: goto BP_LABEL_N(45);
#define __BP_CASE46     __BP_CASE45 case 46: goto BP_LABEL_N(46);
#define __BP_CASE47     __BP_CASE46 case 47: goto BP_LABEL_N(47);
#define __BP_CASE48     __BP_CASE47 case 48: goto BP_LABEL_N(48);
#define __BP_CASE49     __BP_CASE48 case 49: goto BP_LABEL_N(49);
#define __BP_CASE50     __BP_CASE49 case 50: goto BP_LABEL_N(50);
#define __BP_CASE51     __BP_CASE50 case 51: goto BP_LABEL_N(51);
#define __BP_CASE52     __BP_CASE51 case 52: goto BP_LABEL_N(52);
#define __BP_CASE53     __BP_CASE52 case 53: goto BP_LABEL_N(53);
#define __BP_CASE54     __BP_CASE53 case 54: goto BP_LABEL_N(54);
#define __BP_CASE55     __BP_CASE54 case 55: goto BP_LABEL_N(55);
#define __BP_CASE56     __BP_CASE55 case 56: goto BP_LABEL_N(56);
#define __BP_CASE57     __BP_CASE56 case 57: goto BP_LABEL_N(57);
#define __BP_CASE58     __BP_CASE57 case 58: goto BP_LABEL_N(58);
#define __BP_CASE59     __BP_CASE58 case 59: goto BP_LABEL_N(59);
#define __BP_CASE60     __BP_CASE59 case 60: goto BP_LABEL_N(60);
#define __BP_CASE61     __BP_CASE60 case 61: goto BP_LABEL_N(61);
#define __BP_CASE62     __BP_CASE61 case 62: goto BP_LABEL_N(62);
#define __BP_CASE63     __BP_CASE62 case 63: goto BP_LABEL_N(63);
#define __BP_CASE64     __BP_CASE63 case 64: goto BP_LABEL_N(64);
#define __BP_CASE65     __BP_CASE64 case 65: goto BP_LABEL_N(65);
#define __BP_CASE66     __BP_CASE65 case 66: goto BP_LABEL_N(66);
#define __BP_CASE67     __BP_CASE66 case 67: goto BP_LABEL_N(67);
#define __BP_CASE68     __BP_CASE67 case 68: goto BP_LABEL_N(68);
#define __BP_CASE69     __BP_CASE68 case 69: goto BP_LABEL_N(69);
#define __BP_CASE70     __BP_CASE69 case 70: goto BP_LABEL_N(70);
#define __BP_CASE71     __BP_CASE70 case 71: goto BP_LABEL_N(71);
#define __BP_CASE72     __BP_CASE71 case 72: goto BP_LABEL_N(72);
#define __BP_CASE73     __BP_CASE72 case 73: goto BP_LABEL_N(73);
#define __BP_CASE74     __BP_CASE73 case 74: goto BP_LABEL_N(74);
#define __BP_CASE75     __BP_CASE74 case 75: goto BP_LABEL_N(75);
#define __BP_CASE76     __BP_CASE75 case 76: goto BP_LABEL_N(76);
#define __BP_CASE77     __BP_CASE76 case 77: goto BP_LABEL_N(77);
#define __BP_CASE78     __BP_CASE77 case 78: goto BP_LABEL_N(78);
#define __BP_CASE79     __BP_CASE78 case 79: goto BP_LABEL_N(79);
#define __BP_CASE80     __BP_CASE79 case 80: goto BP_LABEL_N(80);
#define __BP_CASE81     __BP_CASE80 case 81: goto BP_LABEL_N(81);
#define __BP_CASE82     __BP_CASE81 case 82: goto BP_LABEL_N(82);
#define __BP_CASE83     __BP_CASE82 case 83: goto BP_LABEL_N(83);
#define __BP_CASE84     __BP_CASE83 case 84: goto BP_LABEL_N(84);
#define __BP_CASE85     __BP_CASE84 case 85: goto BP_LABEL_N(85);
#define __BP_CASE86     __BP_CASE85 case 86: goto BP_LABEL_N(86);
#define __BP_CASE87     __BP_CASE86 case 87: goto BP_LABEL_N(87);
#define __BP_CASE88     __BP_CASE87 case 88: goto BP_LABEL_N(88);
#define __BP_CASE89     __BP_CASE88 case 89: goto BP_LABEL_N(89);
#define __BP_CASE90     __BP_CASE89 case 90: goto BP_LABEL_N(90);
#define __BP_CASE91     __BP_CASE90 case 91: goto BP_LABEL_N(91);
#define __BP_CASE92     __BP_CASE91 case 92: goto BP_LABEL_N(92);
#define __BP_CASE93     __BP_CASE92 case 93: goto BP_LABEL_N(93);
#define __BP_CASE94     __BP_CASE93 case 94: goto BP_LABEL_N(94);
#define __BP_CASE95     __BP_CASE94 case 95: goto BP_LABEL_N(95);
#define __BP_CASE96     __BP_CASE95 case 96: goto BP_LABEL_N(96);
#define __BP_CASE97     __BP_CASE96 case 97: goto BP_LABEL_N(97);
#define __BP_CASE98     __BP_CASE97 case 98: goto BP_LABEL_N(98);
#define __BP_CASE99     __BP_CASE98 case 99: goto BP_LABEL_N(99);
#define __BP_CASE100    __BP_CASE99 case 100: goto BP_LABEL_N(100);
#define __BP_CASE101    __BP_CASE100 case 101: goto BP_LABEL_N(101);
#define __BP_CASE102    __BP_CASE101 case 102: goto BP_LABEL_N(102);
#define __BP_CASE103    __BP_CASE102 case 103: goto BP_LABEL_N(103);
#define __BP_CASE104    __BP_CASE103 case 104: goto BP_LABEL_N(104);
#define __BP_CASE105    __BP_CASE104 case 105: goto BP_LABEL_N(105);
#define __BP_CASE106    __BP_CASE105 case 106: goto BP_LABEL_N(106);
#define __BP_CASE107    __BP_CASE106 case 107: goto BP_LABEL_N(107);
#define __BP_CASE108    __BP_CASE107 case 108: goto BP_LABEL_N(108);
#define __BP_CASE109    __BP_CASE108 case 109: goto BP_LABEL_N(109);
#define __BP_CASE110    __BP_CASE109 case 110: goto BP_LABEL_N(110);
#define __BP_CASE111    __BP_CASE110 case 111: goto BP_LABEL_N(111);
#define __BP_CASE112    __BP_CASE111 case 112: goto BP_LABEL_N(112);
#define __BP_CASE113    __BP_CASE112 case 113: goto BP_LABEL_N(113);
#define __BP_CASE114    __BP_CASE113 case 114: goto BP_LABEL_N(114);
#define __BP_CASE115    __BP_CASE114 case 115: goto BP_LABEL_N(115);
#define __BP_CASE116    __BP_CASE115 case 116: goto BP_LABEL_N(116);
#define __BP_CASE117    __BP_CASE116 case 117: goto BP_LABEL_N(117);
#define __BP_CASE118    __BP_CASE117 case 118: goto BP_LABEL_N(118);
#define __BP_CASE119    __BP_CASE118 case 119: goto BP_LABEL_N(119);
#define __BP_CASE120    __BP_CASE119 case 120: goto BP_LABEL_N(120);
#define __BP_CASE121    __BP_CASE120 case 121: goto BP_LABEL_N(121);
#define __BP_CASE122    __BP_CASE121 case 122: goto BP_LABEL_N(122);
#define __BP_CASE123    __BP_CASE122 case 123: goto BP_LABEL_N(123);
#define __BP_CASE124    __BP_CASE123 case 124: goto BP_LABEL_N(124);
#define __BP_CASE125    __BP_CASE124 case 125: goto BP_LABEL_N(125);
#define __BP_CASE126    __BP_CASE125 case 126: goto BP_LABEL_N(126);
#define __BP_CASE127    __BP_CASE126 case 127: goto BP_LABEL_N(127);
#define __BP_CASE128    __BP_CASE127 case 128: goto BP_LABEL_N(128);
#define __BP_CASE129    __BP_CASE128 case 129: goto BP_LABEL_N(129);
#define __BP_CASE130    __BP_CASE129 case 130: goto BP_LABEL_N(130);
#define __BP_CASE131    __BP_CASE130 case 131: goto BP_LABEL_N(131);
#define __BP_CASE132    __BP_CASE131 case 132: goto BP_LABEL_N(132);
#define __BP_CASE133    __BP_CASE132 case 133: goto BP_LABEL_N(133);
#define __BP_CASE134    __BP_CASE133 case 134: goto BP_LABEL_N(134);
#define __BP_CASE135    __BP_CASE134 case 135: goto BP_LABEL_N(135);
#define __BP_CASE136    __BP_CASE135 case 136: goto BP_LABEL_N(136);
#define __BP_CASE137    __BP_CASE136 case 137: goto BP_LABEL_N(137);
#define __BP_CASE138    __BP_CASE137 case 138: goto BP_LABEL_N(138);
#define __BP_CASE139    __BP_CASE138 case 139: goto BP_LABEL_N(139);
#define __BP_CASE140    __BP_CASE139 case 140: goto BP_LABEL_N(140);
#define __BP_CASE141    __BP_CASE140 case 141: goto BP_LABEL_N(141);
#define __BP_CASE142    __BP_CASE141 case 142: goto BP_LABEL_N(142);
#define __BP_CASE143    __BP_CASE142 case 143: goto BP_LABEL_N(143);
#define __BP_CASE144    __BP_CASE143 case 144: goto BP_LABEL_N(144);
#define __BP_CASE145    __BP_CASE144 case 145: goto BP_LABEL_N(145);
#define __BP_CASE146    __BP_CASE145 case 146: goto BP_LABEL_N(146);
#define __BP_CASE147    __BP_CASE146 case 147: goto BP_LABEL_N(147);
#define __BP_CASE148    __BP_CASE147 case 148: goto BP_LABEL_N(148);
#define __BP_CASE149    __BP_CASE148 case 149: goto BP_LABEL_N(149);
#define __BP_CASE150    __BP_CASE149 case 150: goto BP_LABEL_N(150);
#define __BP_CASE151    __BP_CASE150 case 151: goto BP_LABEL_N(151);
#define __BP_CASE152    __BP_CASE151 case 152: goto BP_LABEL_N(152);
#define __BP_CASE153    __BP_CASE152 case 153: goto BP_LABEL_N(153);
#define __BP_CASE154    __BP_CASE153 case 154: goto BP_LABEL_N(154);
#define __BP_CASE155    __BP_CASE154 case 155: goto BP_LABEL_N(155);
#define __BP_CASE156    __BP_CASE155 case 156: goto BP_LABEL_N(156);
#define __BP_CASE157    __BP_CASE156 case 157: goto BP_LABEL_N(157);
#define __BP_CASE158    __BP_CASE157 case 158: goto BP_LABEL_N(158);
#define __BP_CASE159    __BP_CASE158 case 159: goto BP_LABEL_N(159);
#define __BP_CASE160    __BP_CASE159 case 160: goto BP_LABEL_N(160);
#define __BP_CASE161    __BP_CASE160 case 161: goto BP_LABEL_N(161);
#define __BP_CASE162    __BP_CASE161 case 162: goto BP_LABEL_N(162);
#define __BP_CASE163    __BP_CASE162 case 163: goto BP_LABEL_N(163);
#define __BP_CASE164    __BP_CASE163 case 164: goto BP_LABEL_N(164);
#define __BP_CASE165    __BP_CASE164 case 165: goto BP_LABEL_N(165);
#define __BP_CASE166    __BP_CASE165 case 166: goto BP_LABEL_N(166);
#define __BP_CASE167    __BP_CASE166 case 167: goto BP_LABEL_N(167);
#define __BP_CASE168    __BP_CASE167 case 168: goto BP_LABEL_N(168);
#define __BP_CASE169    __BP_CASE168 case 169: goto BP_LABEL_N(169);
#define __BP_CASE170    __BP_CASE169 case 170: goto BP_LABEL_N(170);
#define __BP_CASE171    __BP_CASE170 case 171: goto BP_LABEL_N(171);
#define __BP_CASE172    __BP_CASE171 case 172: goto BP_LABEL_N(172);
#define __BP_CASE173    __BP_CASE172 case 173: goto BP_LABEL_N(173);
#define __BP_CASE174    __BP_CASE173 case 174: goto BP_LABEL_N(174);
#define __BP_CASE175    __BP_CASE174 case 175: goto BP_LABEL_N(175);
#define __BP_CASE176    __BP_CASE175 case 176: goto BP_LABEL_N(176);
#define __BP_CASE177    __BP_CASE176 case 177: goto BP_LABEL_N(177);
#define __BP_CASE178    __BP_CASE177 case 178: goto BP_LABEL_N(178);
#define __BP_CASE179    __BP_CASE178 case 179: goto BP_LABEL_N(179);
#define __BP_CASE180    __BP_CASE179 case 180: goto BP_LABEL_N(180);
#define __BP_CASE181    __BP_CASE180 case 181: goto BP_LABEL_N(181);
#define __BP_CASE182    __BP_CASE181 case 182: goto BP_LABEL_N(182);
#define __BP_CASE183    __BP_CASE182 case 183: goto BP_LABEL_N(183);
#define __BP_CASE184    __BP_CASE183 case 184: goto BP_LABEL_N(184);
#define __BP_CASE185    __BP_CASE184 case 185: goto BP_LABEL_N(185);
#define __BP_CASE186    __BP_CASE185 case 186: goto BP_LABEL_N(186);
#define __BP_CASE187    __BP_CASE186 case 187: goto BP_LABEL_N(187);
#define __BP_CASE188    __BP_CASE187 case 188: goto BP_LABEL_N(188);
#define __BP_CASE189    __BP_CASE188 case 189: goto BP_LABEL_N(189);
#define __BP_CASE190    __BP_CASE189 case 190: goto BP_LABEL_N(190);
#define __BP_CASE191    __BP_CASE190 case 191: goto BP_LABEL_N(191);
#define __BP_CASE192    __BP_CASE191 case 192: goto BP_LABEL_N(192);
#define __BP_CASE193    __BP_CASE192 case 193: goto BP_LABEL_N(193);
#define __BP_CASE194    __BP_CASE193 case 194: goto BP_LABEL_N(194);
#define __BP_CASE195    __BP_CASE194 case 195: goto BP_LABEL_N(195);
#define __BP_CASE196    __BP_CASE195 case 196: goto BP_LABEL_N(196);
#define __BP_CASE197    __BP_CASE196 case 197: goto BP_LABEL_N(197);
#define __BP_CASE198    __BP_CASE197 case 198: goto BP_LABEL_N(198);
#define __BP_CASE199    __BP_CASE198 case 199: goto BP_LABEL_N(199);
#define __BP_CASE200    __BP_CASE199 case 200: goto BP_LABEL_N(200);
#define __BP_CASE201    __BP_CASE200 case 201: goto BP_LABEL_N(201);
#define __BP_CASE202    __BP_CASE201 case 202: goto BP_LABEL_N(202);
#define __BP_CASE203    __BP_CASE202 case 203: goto BP_LABEL_N(203);
#define __BP_CASE204    __BP_CASE203 case 204: goto BP_LABEL_N(204);
#define __BP_CASE205    __BP_CASE204 case 205: goto BP_LABEL_N(205);
#define __BP_CASE206    __BP_CASE205 case 206: goto BP_LABEL_N(206);
#define __BP_CASE207    __BP_CASE206 case 207: goto BP_LABEL_N(207);
#define __BP_CASE208    __BP_CASE207 case 208: goto BP_LABEL_N(208);
#define __BP_CASE209    __BP_CASE208 case 209: goto BP_LABEL_N(209);
#define __BP_CASE210    __BP_CASE209 case 210: goto BP_LABEL_N(210);
#define __BP_CASE211    __BP_CASE210 case 211: goto BP_LABEL_N(211);
#define __BP_CASE212    __BP_CASE211 case 212: goto BP_LABEL_N(212);
#define __BP_CASE213    __BP_CASE212 case 213: goto BP_LABEL_N(213);
#define __BP_CASE214    __BP_CASE213 case 214: goto BP_LABEL_N(214);
#define __BP_CASE215    __BP_CASE214 case 215: goto BP_LABEL_N(215);
#define __BP_CASE216    __BP_CASE215 case 216: goto BP_LABEL_N(216);
#define __BP_CASE217    __BP_CASE216 case 217: goto BP_LABEL_N(217);
#define __BP_CASE218    __BP_CASE217 case 218: goto BP_LABEL_N(218);
#define __BP_CASE219    __BP_CASE218 case 219: goto BP_LABEL_N(219);
#define __BP_CASE220    __BP_CASE219 case 220: goto BP_LABEL_N(220);
#define __BP_CASE221    __BP_CASE220 case 221: goto BP_LABEL_N(221);
#define __BP_CASE222    __BP_CASE221 case 222: goto BP_LABEL_N(222);
#define __BP_CASE223    __BP_CASE222 case 223: goto BP_LABEL_N(223);
#define __BP_CASE224    __BP_CASE223 case 224: goto BP_LABEL_N(224);
#define __BP_CASE225    __BP_CASE224 case 225: goto BP_LABEL_N(225);
#define __BP_CASE226    __BP_CASE225 case 226: goto BP_LABEL_N(226);
#define __BP_CASE227    __BP_CASE226 case 227: goto BP_LABEL_N(227);
#define __BP_CASE228    __BP_CASE227 case 228: goto BP_LABEL_N(228);
#define __BP_CASE229    __BP_CASE228 case 229: goto BP_LABEL_N(229);
#define __BP_CASE230    __BP_CASE229 case 230: goto BP_LABEL_N(230);
#define __BP_CASE231    __BP_CASE230 case 231: goto BP_LABEL_N(231);
#define __BP_CASE232    __BP_CASE231 case 232: goto BP_LABEL_N(232);
#define __BP_CASE233    __BP_CASE232 case 233: goto BP_LABEL_N(233);
#define __BP_CASE234    __BP_CASE233 case 234: goto BP_LABEL_N(234);
#define __BP_CASE235    __BP_CASE234 case 235: goto BP_LABEL_N(235);
#define __BP_CASE236    __BP_CASE235 case 236: goto BP_LABEL_N(236);
#define __BP_CASE237    __BP_CASE236 case 237: goto BP_LABEL_N(237);
#define __BP_CASE238    __BP_CASE237 case 238: goto BP_LABEL_N(238);
#define __BP_CASE239    __BP_CASE238 case 239: goto BP_LABEL_N(239);
#define __BP_CASE240    __BP_CASE239 case 240: goto BP_LABEL_N(240);
#define __BP_CASE241    __BP_CASE240 case 241: goto BP_LABEL_N(241);
#define __BP_CASE242    __BP_CASE241 case 242: goto BP_LABEL_N(242);
#define __BP_CASE243    __BP_CASE242 case 243: goto BP_LABEL_N(243);
#define __BP_CASE244    __BP_CASE243 case 244: goto BP_LABEL_N(244);
#define __BP_CASE245    __BP_CASE244 case 245: goto BP_LABEL_N(245);
#define __BP_CASE246    __BP_CASE245 case 246: goto BP_LABEL_N(246);
#define __BP_CASE247    __BP_CASE246 case 247: goto BP_LABEL_N(247);
#define __BP_CASE248    __BP_CASE247 case 248: goto BP_LABEL_N(248);
#define __BP_CASE249    __BP_CASE248 case 249: goto BP_LABEL_N(249);
#define __BP_CASE250    __BP_CASE249 case 250: goto BP_LABEL_N(250);
#define __BP_CASE251    __BP_CASE250 case 251: goto BP_LABEL_N(251);
#define __BP_CASE252    __BP_CASE251 case 252: goto BP_LABEL_N(252);
#define __BP_CASE253    __BP_CASE252 case 253: goto BP_LABEL_N(253);
#define __BP_CASE254    __BP_CASE253 case 254: goto BP_LABEL_N(254);
#define __BP_CASE255    __BP_CASE254 case 255: goto BP_LABEL_N(255);

/* then define a __BP_CAT_connection macro for use by __BP_CASEN */
/* 再定义一个__BP_CAT_连接宏，供__BP_CASEN使用 */
#define __BP_CAT_(x, y)     __BP_CAT_1(x, y)
#define __BP_CAT_1(x, y)    x##y

/* because __BP_CASE contains the __BP_CAT macro,
 * you cannot use the __BP_CAT macro again,
 * so use the __BP_CAT_ macro */
/* 由于__BP_CASE中包含了__BP_CAT宏，
 * 因此不可再次使用__BP_CAT宏，
 * 故使用__BP_CAT_宏 */
#define __BP_CASEN(bp_nums) __BP_CAT_(__BP_CASE, bp_nums)


/*********************************************************
 *@brief: 
 ***start a bp coroutine
 *
 *@contract: 
 ***1. "bp_nums" must be a pure number, or a macro that is expanded to be a pure number
 ***2. "bp_nums" indicates the number of breakpoints contained between bp_begin() and bp_end()
 ***3. the breakpoint number between bp_begin() and bp_end() is continuous from 1 to n,
 ***   and must not be missed or redundant.
 *
 *@parameter:
 *[bp_nums]: indicates the number of breakpoints contained between bp_begin() and bp_end()
 *[bp]: variable(1 byte) for recording the current breakpoint position, the life cycle is global
 *********************************************************/
/*********************************************************
 *@简要：
 ***开始一个bp协程
 *
 *@约定：
 ***1、bp_nums必须是纯数字，或者展开后为纯数字的宏
 ***2、bp_nums表示从bp_begin()到bp_end()之间包含的断点个数
 ***3、bp_begin()到bp_end()之间的断点号从1到n连续，不可遗漏与多余
 *
 *@参数：
 *[bp_nums]：表示bp_begin()到bp_end()之间包含的断点个数
 *[bp]：用于记录当前断点位置的生命周期为全局的变量（一个字节）
 **********************************************************/
#define bp_begin(bp_nums, bp)   __BP_HEADER(bp) __BP_CASEN(bp_nums) __BP_TAIL


/*********************************************************
 *@brief: 
 ***together with bp_begin(), constitutes the scope of the coroutine
 *
 *@parameter:
 *[bp]: variable(1 byte) for recording the current breakpoint position, the life cycle is global
 *********************************************************/
/*********************************************************
 *@简要：
 ***与bp_begin()组成协程的作用域
 *
 *@参数：
 *[bp]：用于记录当前断点位置的生命周期为全局的变量（一个字节）
 **********************************************************/
#define bp_end(bp)  \
    do { BP_LABEL_END: (bp) = BP_INIT_VAL; } while (0)


/************************************************************
 *@brief:
 ***record the breakpoint number in the bp variable
 *
 *@parameter:
 *[bp_num]: recorded breakpoint number
 *[bp]: variable(1 byte) for recording the current breakpoint position, the life cycle is global
 *************************************************************/
/************************************************************
 *@简介：
 ***将断点号记录在bp变量之中
 *
 *@参数：
 *[bp_num]：记录的断点号
 *[bp]：用于记录当前断点位置的生命周期为全局的变量（一个字节）
 *************************************************************/
#define bp_set(bp_num, bp)  ((int)((bp) = (bp_num)))


/************************************************************
 *@brief:
 ***create a recovery location with a breakpoint of bp_num,
 ***and bp_begin() will jump to the appropriate recovery location
 ***based on the breakpoint number in the bp variable.
 *
 *@contract:
 ***1. create a recovery location with a different bp_num each time
 ***2. the breakpoint number between bp_begin() and bp_end() is continuous from 1 to n,
 ***   and must not be missed or redundant.
 ***3. "bp_nums" must be a pure number, or a macro that is expanded to be a pure number
 * 
 *@parameter:
 *[bp_num]: corresponding breakpoint number
 *************************************************************/
/************************************************************
 *@简介：
 ***创建一个断点号为bp_num的恢复位置，bp_begin()会根据bp变量中的断点号
 ***跳转到相应的恢复位置。
 *
 *@约定：
 ***1、每次都使用不同的bp_num创建恢复位置
 ***2、bp_begin()到bp_end()之间的断点号从1到n连续，不可遗漏与多余
 ***3、bp_nums必须是纯数字，或者展开后为纯数字的宏
 *
 *@参数：
 *[bp_num]：对应的断点号
 *************************************************************/
#define bp_restore_point(bp_num)        BP_LABEL_N(bp_num)


/*************************************************************************
 * BP initialization
 * BP初始化
 *************************************************************************
 */
#define BP_INIT_VAL     0
#define bp_init(bp)     ((bp) = BP_INIT_VAL)


/*********************************************************
 *@description: 
 ***BP yield series
 *
 *@note:
 ***the bp_yield family of functions consists of bp_set and bp_restore_point,
 ***so you need to follow the usage contract of bp_restore_point.
 ***as follows:
 ***1. "bp_nums" must be a pure number, or a macro that is expanded to be a pure number
 ***2. the breakpoint number between bp_begin() and bp_end() is continuous from 1 to n,
 ***   and must not be missed or redundant.
 *********************************************************
 *@说明：
 ***BP yield系列函数
 *
 *@注意：
 ***bp_yield系列函数由bp_set与bp_restore_point组成，因此需要
 ***遵守bp_restore_point的使用约定。
 ***如下：
 ***1、bp_nums必须是纯数字，或者展开后为纯数字的宏
 ***2、bp_begin()到bp_end()之间的断点号从1到n连续，不可遗漏与多余
 *********************************************************/

/* Active return, followed by a statement or value, 
 * and restore the current execution position in the next call */
/* 主动返回，后面可以跟一条语句或值，并在下次调用恢复当前执行位置 */
#define bp_yield(bp_num, bp)            \
    if (0)                              \
    {                                   \
        bp_restore_point(bp_num):;      \
    }                                   \
    else while (bp_set(bp_num, bp))     \
        return

/*************************************************************************
 * BP other series
 * BP 其他系列
 *************************************************************************
 */

/* jump out of the BP_BEGIN to BP_END statement */
/* 跳出BP_BEGIN到BP_END包含的语句之外 */
#define bp_break    \
    do {goto BP_LABEL_END; } while (0)

/* re-execution after BP_BEGIN */
/* 从BP_BEGIN之后重新执行 */
#define bp_continue \
    do { goto BP_LABEL_START; } while (0)

/* exit BP_BEGIN-BP_END statement block,
 * the next call will be re-executed */
/* 退出BP_BEGIN-BP_END语句块，下次调用将重新执行 */
#define bp_exit(bp) \
    while (((bp) = BP_INIT_VAL) || 1) return

/* return directly without
 * modifying the breakpoint location */
/* 直接返回，不修改断点位置 */
#define bp_ret  return

/************************************************************
 * define a bpd pointer to use BPD series macros,
 * omit bp parameter
 * usage：
 *
 *  unsigned char *bpd = &ctx->bp;
 *
 *  bpd_begin(1)
 *
 *  bpd_yield(1);
 *
 *  bpd_end();
 *
 ************************************************************/
/************************************************************
 * 定义一个bpd指针，可使用BPD系列宏，省略bp参数
 * 例：
 *
 *  unsigned char *bpd = &ctx->bp;
 *
 *  bpd_begin(1)
 *
 *  bpd_yield(1);
 *
 *  bpd_end();
 *
 ************************************************************/

/* default bpd as the bp parameter
 * of the bp series macro */
/* 将bpd默认作为bp系列宏的bp参数 */
#define BP_DEFAULT                                  *bpd

#define BPD_INIT_VAL                                0

#define bpd_init(bp)                                ((BP_DEFAULT) = BP_INIT_VAL)

#define bpd_restore_point(point)                    bp_restore_point(point)

#define bpd_begin(bp_nums)                          bp_begin(bp_nums, (BP_DEFAULT))

#define bpd_end()                                   bp_end((BP_DEFAULT))

#define bpd_set(bp_num)                             bp_set(bp_num, (BP_DEFAULT))

#define bpd_yield(bp_num)                           bp_yield(bp_num, (BP_DEFAULT))

#define bpd_break                                   bp_break

#define bpd_continue                                bp_continue

#define bpd_exit()                                  bp_exit((BP_DEFAULT))

#define bpd_ret                                     bp_ret

#endif /* __BP_H__ */
