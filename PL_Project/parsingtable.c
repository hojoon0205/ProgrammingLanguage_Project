#include <stdio.h>

//int ParseTable[57][34] = 0;

void Table(void)
{
	//word 
	ParseTable[2][0] = 106;
	ParseTable[3][0] = 206;
	ParseTable[4][0] = 207;
	ParseTable[5][0] = 208;
	ParseTable[7][0] = 109;
	ParseTable[10][0] = 210;
	ParseTable[11][0] = 114;
	ParseTable[13][0] = 202;
	ParseTable[15][0] = 122;
	ParseTable[16][0] = 122;
	ParseTable[17][0] = 201;
	ParseTable[18][0] = 212;
	ParseTable[19][0] = 109;
	ParseTable[20][0] = 132;
	ParseTable[21][0] = 132;
	ParseTable[23][0] = 132;
	ParseTable[24][0] = 209;
	ParseTable[25][0] = 211;
	ParseTable[29][0] = 220;
	ParseTable[30][0] = 222;
	ParseTable[31][0] = 224;
	ParseTable[32][0] = 225;
	ParseTable[33][0] = 210;
	ParseTable[34][0] = 132;
	ParseTable[35][0] = 109; 
	ParseTable[37][0] = 203;
	ParseTable[38][0] = 210;
	ParseTable[39][0] = 132;
	ParseTable[40][0] = 132;
	ParseTable[41][0] = 132;
	ParseTable[42][0] = 132;
	ParseTable[43][0] = 214;
	ParseTable[46][0] = 217;
	ParseTable[48][0] = 218;
	ParseTable[49][0] = 219;
	ParseTable[50][0] = 221;
	ParseTable[51][0] = 223;
	ParseTable[52][0] = 215;
	ParseTable[54][0] = 210;
	ParseTable[55][0] = 216;
	ParseTable[56][0] = 213;
	//(
	ParseTable[6][1] = 107;
	ParseTable[22][1] = 135;
	//)
	ParseTable[8][2] = 110;
	ParseTable[9][2] = 205;
	ParseTable[14][2] = 204;
	ParseTable[45][2] = 153;
	//;
	ParseTable[9][3] = 205;
	ParseTable[14][3] = 204;
	ParseTable[26][3] = 137;
	ParseTable[29][3] = 220;
	ParseTable[30][3] = 222;
	ParseTable[31][3] = 224;
	ParseTable[32][3] = 225;
	ParseTable[36][3] = 146;
	ParseTable[44][3] = 152;
	ParseTable[50][3] = 221;
	ParseTable[51][3] = 223;
	ParseTable[53][3] = 155;
	//,
	ParseTable[8][4] = 111;
	ParseTable[9][4] = 205;
	ParseTable[14][4] = 204;
	ParseTable[26][4] = 111;
	ParseTable[45][4] = 111;
	//int
	ParseTable[0][5] = 103;
	ParseTable[13][5] = 202;
	ParseTable[15][5] = 103;
	ParseTable[17][5] = 201;
	ParseTable[37][5] = 203;
	//char
	ParseTable[0][6] = 104;
	ParseTable[13][6] = 202;
	ParseTable[15][6] = 104;
	ParseTable[17][6] = 201;
	ParseTable[37][6] = 203;
	//void
	ParseTable[0][7] = 105;
	ParseTable[13][7] = 202;
	ParseTable[15][7] = 105;
	ParseTable[17][7] = 201;
	ParseTable[37][7] = 203;
	//{
	ParseTable[10][8] = 113;
	ParseTable[29][8] = 220;
	ParseTable[30][8] = 222;
	ParseTable[31][8] = 224;
	ParseTable[32][8] = 225;
	ParseTable[33][8] = 113;
	ParseTable[38][8] = 113;
	ParseTable[48][8] = 218;
	ParseTable[49][8] = 219;
	ParseTable[50][8] = 221;
	ParseTable[51][8] = 223;
	ParseTable[54][8] = 113;
	//}
	ParseTable[10][9] = 210;
	ParseTable[16][9] = 124;
	ParseTable[18][9] = 212;
	ParseTable[24][9] = 209;
	ParseTable[25][9] = 211;
	ParseTable[29][9] = 220;
	ParseTable[30][9] = 222;
	ParseTable[31][9] = 224;
	ParseTable[32][9] = 225;
	ParseTable[33][9] = 210;
	ParseTable[38][9] = 210;
	ParseTable[43][9] = 214;
	ParseTable[46][9] = 217;
	ParseTable[48][9] = 218;
	ParseTable[49][9] = 219;
	ParseTable[50][9] = 221;
	ParseTable[51][9] = 223;
	ParseTable[52][9] = 215;
	ParseTable[54][9] = 210;
	ParseTable[55][9] = 216;
	ParseTable[56][9] = 213;
	//if
	ParseTable[10][10] = 210;
	ParseTable[13][10] = 202;
	ParseTable[15][10] = 120;
	ParseTable[16][10] = 120;
	ParseTable[17][10] = 201;
	ParseTable[18][10] = 212;
	ParseTable[24][10] = 209;
	ParseTable[25][10] = 211;
	ParseTable[29][10] = 220;
	ParseTable[30][10] = 222;
	ParseTable[31][10] = 224;
	ParseTable[32][10] = 225;
	ParseTable[33][10] = 210;
	ParseTable[37][10] = 203;
	ParseTable[38][10] = 210;
	ParseTable[43][10] = 214;
	ParseTable[46][10] = 217;
	ParseTable[48][10] = 218;
	ParseTable[49][10] = 219;
	ParseTable[50][10] = 221;
	ParseTable[51][10] = 223;
	ParseTable[52][10] = 215;
	ParseTable[54][10] = 210;
	ParseTable[55][10] = 216;
	ParseTable[56][10] = 213;
	//then
	ParseTable[27][11] = 138;
	ParseTable[29][11] = 220;
	ParseTable[30][11] = 222;
	ParseTable[31][11] = 224;
	ParseTable[32][11] = 225;
	ParseTable[48][11] = 218;
	ParseTable[49][11] = 219;
	ParseTable[50][11] = 221;
	ParseTable[51][11] = 223;	
	//else
	ParseTable[10][12] = 210;
	ParseTable[24][12] = 209;
	ParseTable[33][12] = 210;
	ParseTable[38][12] = 210;
	ParseTable[47][12] = 154;
	ParseTable[54][12] = 210;
	//while
	ParseTable[10][13] = 210;
	ParseTable[13][13] = 202;
	ParseTable[15][13] = 121;
	ParseTable[16][13] = 121;
	ParseTable[24][13] = 209;
	ParseTable[25][13] = 211;
	ParseTable[29][13] = 220;
	ParseTable[30][13] = 222;
	ParseTable[31][13] = 224;
	ParseTable[32][13] = 225;
	ParseTable[33][13] = 210;
	ParseTable[37][13] = 203;
	ParseTable[38][13] = 210;
	ParseTable[43][13] = 214;
	ParseTable[46][13] = 217;
	ParseTable[48][13] = 218;
	ParseTable[49][13] = 219;
	ParseTable[50][13] = 221;
	ParseTable[51][13] = 223;
	ParseTable[52][13] = 215;
	ParseTable[54][13] = 210;
	ParseTable[55][13] = 216;
	ParseTable[56][13] = 213;
	
	//=
	ParseTable[22][14] = 134;
	//return
	ParseTable[10][15] = 210;
	ParseTable[13][15] = 202;
	ParseTable[15][15] = 123;
	ParseTable[16][15] = 123;
	ParseTable[24][15] = 209;
	ParseTable[25][15] = 211;
	ParseTable[29][15] = 220;
	ParseTable[30][15] = 222;
	ParseTable[31][15] = 224;
	ParseTable[32][15] = 225;
	ParseTable[33][15] = 210;
	ParseTable[37][15] = 203;
	ParseTable[38][15] = 210;
	ParseTable[43][15] = 214;
	ParseTable[46][15] = 217;
	ParseTable[48][15] = 218;
	ParseTable[49][15] = 219;
	ParseTable[50][15] = 221;
	ParseTable[51][15] = 223;
	ParseTable[52][15] = 215;
	ParseTable[54][15] = 210;
	ParseTable[55][15] = 216;
	ParseTable[56][15] = 213;
	//>
	ParseTable[28][16] = 139;
	ParseTable[29][16] = 220;
	ParseTable[30][16] = 222;
	ParseTable[31][16] = 224;
	ParseTable[32][16] = 225;
	ParseTable[50][16] = 221;
	ParseTable[51][16] = 223;
	//==
	ParseTable[28][17] = 140;
	ParseTable[29][17] = 220;
	ParseTable[30][17] = 222;
	ParseTable[31][17] = 224;
	ParseTable[32][17] = 225;
	ParseTable[50][17] = 221;
	ParseTable[51][17] = 223;
	//+
	ParseTable[29][18] = 141;
	ParseTable[30][18] = 222;
	ParseTable[31][18] = 224;
	ParseTable[32][18] = 225;
	ParseTable[51][18] = 223;
	//*
	ParseTable[30][19] = 142;
	ParseTable[31][19] = 224;
	ParseTable[32][19] = 225;
	//num
	ParseTable[20][20] = 131;
	ParseTable[21][20] = 131;
	ParseTable[23][20] = 131;
	ParseTable[34][20] = 131;
	ParseTable[39][20] = 131;
	ParseTable[40][20] = 131;
	ParseTable[41][20] = 131;
	ParseTable[42][20] = 131;
	//$
	ParseTable[1][21] = 10000;
	ParseTable[10][21] = 210;
	ParseTable[12][21] = 200;
	ParseTable[24][21] = 209;//211
	ParseTable[33][21] = 210;
	ParseTable[38][21] = 210;
	ParseTable[54][21] = 210;
	//prog~fact
	ParseTable[0][22]= 1;
	ParseTable[0][23]= 2;
	ParseTable[7][24]= 8;
	ParseTable[10][25] = 12;
	ParseTable[13][26] = 15;
	ParseTable[15][23] = 19;
	ParseTable[15][27] = 17;
	ParseTable[15][28] = 16;
	ParseTable[15][29] = 18;
	ParseTable[16][29] = 25;
	ParseTable[19][24] = 26;
	ParseTable[20][30] = 27;
	ParseTable[20][31] = 28;
	ParseTable[20][32] = 29;
	ParseTable[20][33] = 30;
	ParseTable[21][30] = 33;
	ParseTable[21][31] = 28;
	ParseTable[21][32] = 29;
	ParseTable[21][33] = 30;
	ParseTable[23][31] = 36;
	ParseTable[23][32] = 29;
	ParseTable[23][33] = 30;
	ParseTable[33][25] = 43;
	ParseTable[34][31] = 44;
	ParseTable[34][32] = 29;
	ParseTable[34][33] = 30;
	ParseTable[35][24] = 45;
	ParseTable[38][25] = 47;
	ParseTable[39][31] = 48;
	ParseTable[39][32] = 29;
	ParseTable[39][33] = 30;
	ParseTable[40][31] = 49;
	ParseTable[40][32] = 29;
	ParseTable[40][33] = 30;
	ParseTable[41][32] = 50;
	ParseTable[41][33] = 30;
	ParseTable[42][33] = 51;
	ParseTable[54][25] = 56;
	
}

//accept =10000
//shift = +100
//reduce = +200 
