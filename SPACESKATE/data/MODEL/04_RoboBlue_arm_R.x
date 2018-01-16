xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 220;
 -15.56175;0.48948;3.24456;,
 -15.92520;0.48354;6.25365;,
 -15.90228;-0.92700;6.25365;,
 -15.53886;-0.92106;3.24456;,
 -15.53886;-0.92106;3.24456;,
 -15.90228;-0.92700;6.25365;,
 -16.58913;-0.93819;4.64421;,
 -15.56175;0.48948;3.24456;,
 -16.61202;0.47244;4.64421;,
 -15.92520;0.48354;6.25365;,
 -15.12957;-0.78372;6.21591;,
 -14.52651;-1.58478;5.27439;,
 -14.12970;-1.20399;5.03841;,
 -14.03919;-0.76602;5.46378;,
 -15.11643;-1.59429;6.21591;,
 -14.57157;1.19352;5.27439;,
 -15.14838;0.37329;6.21591;,
 -14.05794;0.39102;5.46378;,
 -14.16267;0.82578;5.03841;,
 -15.16155;1.18392;6.21591;,
 -15.96120;-0.79725;2.59230;,
 -14.65197;-0.77598;2.79354;,
 -14.54724;-1.21077;3.21891;,
 -15.00663;-1.59258;3.18231;,
 -15.94806;-1.60788;2.59230;,
 -14.67075;0.38109;2.79354;,
 -15.97998;0.35979;2.59230;,
 -15.05175;1.18575;3.18231;,
 -14.58021;0.81897;3.21891;,
 -15.99315;1.17042;2.59230;,
 -4.67505;-1.38882;1.91160;,
 -4.40853;-1.38453;3.07305;,
 -4.56129;-1.38699;3.18354;,
 -0.80343;-1.32597;2.32536;,
 -0.98919;-1.32903;2.29221;,
 -1.25661;-1.33335;1.12689;,
 -4.06686;-1.37895;3.48288;,
 -4.03713;-1.37847;3.29790;,
 -2.63091;-1.35567;2.97510;,
 -3.29919;-1.36650;0.06309;,
 -4.70544;-1.38933;0.38589;,
 -4.81284;-1.39107;0.23238;,
 -1.22463;-1.33284;2.65224;,
 -1.11759;-1.33110;2.80584;,
 -1.86360;-1.34322;-0.44466;,
 -1.89291;-1.34364;-0.25974;,
 -4.94157;-1.39311;0.75018;,
 -5.12721;-1.39611;0.71742;,
 -1.37133;-1.33521;-0.14931;,
 -1.52403;-1.33767;-0.03840;,
 -15.99315;1.17042;2.59230;,
 -16.62324;1.16019;4.64421;,
 -15.05175;1.18575;3.18231;,
 -14.53140;1.19412;4.16400;,
 -14.57157;1.19352;5.27439;,
 -15.16155;1.18392;6.21591;,
 -16.14312;1.16802;6.73629;,
 -14.52651;-1.58478;5.27439;,
 -16.57815;-1.61811;4.64421;,
 -14.48634;-1.58412;4.16400;,
 -16.09800;-1.61031;6.73629;,
 -15.11643;-1.59429;6.21591;,
 -15.00663;-1.59258;3.18231;,
 -15.94806;-1.60788;2.59230;,
 -16.14312;1.16802;6.73629;,
 -16.09800;-1.61031;6.73629;,
 -14.48634;-1.58412;4.16400;,
 -5.38020;-1.06194;1.11456;,
 -4.96263;-1.05513;2.93403;,
 -14.12970;-1.20399;5.03841;,
 -14.54724;-1.21077;3.21891;,
 -14.65197;-0.77598;2.79354;,
 -5.48484;-0.62712;0.68919;,
 -4.87206;-0.61722;3.35940;,
 -14.03919;-0.76602;5.46378;,
 -14.12970;-1.20399;5.03841;,
 -4.96263;-1.05513;2.93403;,
 -14.67075;0.38109;2.79354;,
 -5.50365;0.52989;0.68919;,
 -14.53140;1.19412;4.16400;,
 -14.58021;0.81897;3.21891;,
 -5.41311;0.96783;1.11456;,
 -14.05794;0.39102;5.46378;,
 -4.89087;0.53985;3.35940;,
 -14.16267;0.82578;5.03841;,
 -4.99557;0.97464;2.93403;,
 -5.47779;-1.06353;0.68919;,
 -5.38020;-1.06194;1.11456;,
 -5.48484;-0.62712;0.68919;,
 -5.12721;-1.39611;0.71742;,
 -4.96263;-1.05513;2.93403;,
 -4.56129;-1.38699;3.18354;,
 -4.86495;-1.05357;3.35937;,
 -4.87206;-0.61722;3.35940;,
 -4.97142;-1.05528;-0.09195;,
 -4.81284;-1.39107;0.23238;,
 -1.87386;-1.00500;-0.80301;,
 -1.86360;-1.34322;-0.44466;,
 -4.81284;-1.39107;0.23238;,
 -5.50365;0.52989;0.68919;,
 -5.00436;0.97449;-0.09195;,
 -1.90680;1.02480;-0.80301;,
 -5.51076;0.96627;0.68919;,
 -5.17119;1.31016;0.71742;,
 -4.85682;1.31526;0.23238;,
 -5.41311;0.96783;1.11456;,
 -4.85682;1.31526;0.23238;,
 -1.90752;1.36314;-0.44466;,
 -6.83115;1.28319;0.55068;,
 -5.17119;1.31016;0.71742;,
 -4.60524;1.31934;3.18354;,
 -5.95206;1.29750;4.38129;,
 -6.00237;1.29666;-0.77217;,
 -4.85682;1.31526;0.23238;,
 -4.11081;1.32735;3.48288;,
 -4.62942;1.31895;5.21025;,
 -0.79929;1.38114;4.33104;,
 -1.16154;1.37526;2.80584;,
 -0.84735;1.38036;2.32536;,
 0.02958;1.39461;3.00819;,
 -1.41528;1.37112;-0.14931;,
 -0.84957;1.38030;-0.82245;,
 -1.90752;1.36314;-0.44466;,
 -2.17221;1.35882;-1.65141;,
 -5.98227;3.15804;4.38129;,
 -5.95206;1.29750;4.38129;,
 -4.62942;1.31895;5.21025;,
 -4.65963;3.17949;5.21025;,
 -0.82950;3.24162;4.33104;,
 -0.79929;1.38114;4.33104;,
 0.02958;1.39461;3.00819;,
 -0.00063;3.25515;3.00819;,
 -0.87978;3.24084;-0.82245;,
 -0.84957;1.38030;-0.82245;,
 -2.17221;1.35882;-1.65141;,
 -2.20245;3.21939;-1.65141;,
 -6.03258;3.15720;-0.77217;,
 -6.00237;1.29666;-0.77217;,
 -6.83115;1.28319;0.55068;,
 -6.86139;3.14373;0.55068;,
 -5.95206;1.29750;4.38129;,
 -5.98227;3.15804;4.38129;,
 -2.07690;3.91854;1.80906;,
 -4.65822;3.87663;2.40162;,
 -3.47196;3.89592;3.14505;,
 -2.82024;3.90651;2.99547;,
 -2.22645;3.91614;1.15725;,
 -4.80780;3.87420;1.74981;,
 -5.98227;3.15804;4.38129;,
 -4.65963;3.17949;5.21025;,
 -0.82950;3.24162;4.33104;,
 -0.00063;3.25515;3.00819;,
 -0.87978;3.24084;-0.82245;,
 -2.20245;3.21939;-1.65141;,
 -3.41274;3.89685;0.41379;,
 -4.06443;3.88629;0.56337;,
 -6.03258;3.15720;-0.77217;,
 -6.86139;3.14373;0.55068;,
 -4.06872;-1.04064;3.84147;,
 -4.86495;-1.05357;3.35937;,
 -4.56129;-1.38699;3.18354;,
 -4.06686;-1.37895;3.48288;,
 -4.06686;-1.37895;3.48288;,
 -1.11759;-1.33110;2.80584;,
 -0.97113;-0.99033;3.13041;,
 -1.11759;-1.33110;2.80584;,
 -0.80343;-1.32597;2.32536;,
 -0.46359;-0.98208;2.35431;,
 -0.80343;-1.32597;2.32536;,
 -1.37133;-1.33521;-0.14931;,
 -1.07874;-0.99210;-0.32595;,
 -1.37133;-1.33521;-0.14931;,
 -1.86360;-1.34322;-0.44466;,
 -0.49656;1.04769;2.35431;,
 -1.00407;1.03947;3.13041;,
 -1.11165;1.03770;-0.32595;,
 -4.87206;-0.61722;3.35940;,
 -4.10166;0.98916;3.84147;,
 -4.89087;0.53985;3.35940;,
 -4.99557;0.97464;2.93403;,
 -4.89792;0.97623;3.35937;,
 -4.60524;1.31934;3.18354;,
 -4.11081;1.32735;3.48288;,
 -4.60524;1.31934;3.18354;,
 -4.89792;0.97623;3.35937;,
 -4.89087;0.53985;3.35940;,
 -1.90752;1.36314;-0.44466;,
 -1.41528;1.37112;-0.14931;,
 -1.41528;1.37112;-0.14931;,
 -0.84735;1.38036;2.32536;,
 -0.84735;1.38036;2.32536;,
 -1.16154;1.37526;2.80584;,
 -1.16154;1.37526;2.80584;,
 -4.11081;1.32735;3.48288;,
 -4.94157;-1.39311;0.75018;,
 -4.41291;-3.22401;0.70710;,
 -4.43676;-3.32022;1.70298;,
 -4.67505;-1.38882;1.91160;,
 -4.67505;-1.38882;1.91160;,
 -4.43676;-3.32022;1.70298;,
 -4.13286;-3.25314;2.67099;,
 -4.40853;-1.38453;3.07305;,
 -3.51453;-3.01440;3.18759;,
 -4.03713;-1.37847;3.29790;,
 -2.49390;-2.66817;3.22233;,
 -2.63091;-1.35567;2.97510;,
 -1.48749;-2.36424;2.90259;,
 -1.22463;-1.33284;2.65224;,
 -0.96936;-2.21256;2.26062;,
 -0.98919;-1.32903;2.29221;,
 -0.93168;-2.13567;1.26369;,
 -1.25661;-1.33335;1.12689;,
 -1.24092;-2.16834;0.29457;,
 -1.52403;-1.33767;-0.03840;,
 -1.88409;-2.33610;-0.22140;,
 -1.89291;-1.34364;-0.25974;,
 -2.93346;-2.60808;-0.25581;,
 -3.29919;-1.36650;0.06309;,
 -3.92280;-2.97723;0.06426;,
 -4.70544;-1.38933;0.38589;;
 
 126;
 4;0,1,2,3;,
 3;4,5,6;,
 3;7,8,9;,
 4;10,11,12,13;,
 3;14,11,10;,
 4;15,16,17,18;,
 3;16,15,19;,
 4;17,16,10,13;,
 4;20,21,22,23;,
 3;20,23,24;,
 4;25,26,27,28;,
 3;29,27,26;,
 4;21,20,26,25;,
 3;30,31,32;,
 3;33,34,35;,
 3;36,37,38;,
 3;39,40,41;,
 4;32,31,37,36;,
 4;36,38,42,43;,
 4;43,42,34,33;,
 4;44,45,39,41;,
 4;41,40,46,47;,
 4;48,49,45,44;,
 4;33,35,49,48;,
 4;47,46,30,32;,
 3;50,51,52;,
 3;53,51,54;,
 3;55,51,56;,
 3;54,51,55;,
 3;52,51,53;,
 3;57,58,59;,
 3;60,58,61;,
 3;61,58,57;,
 3;59,58,62;,
 3;62,58,63;,
 3;16,19,64;,
 3;65,14,10;,
 4;65,10,16,64;,
 3;66,23,22;,
 3;12,11,66;,
 3;22,12,66;,
 4;67,68,69,70;,
 4;70,71,72,67;,
 4;73,74,75,76;,
 4;71,77,78,72;,
 3;79,15,18;,
 3;28,27,79;,
 3;18,28,79;,
 4;80,81,78,77;,
 4;82,74,73,83;,
 4;80,84,85,81;,
 4;82,83,85,84;,
 3;86,87,88;,
 3;87,86,89;,
 4;90,87,89,91;,
 3;91,92,90;,
 3;93,90,92;,
 4;94,95,89,86;,
 4;96,97,98,94;,
 3;94,86,88;,
 4;94,88,99,100;,
 4;96,94,100,101;,
 4;100,102,103,104;,
 3;99,105,102;,
 4;101,100,106,107;,
 3;99,102,100;,
 4;108,109,110,111;,
 4;109,108,112,113;,
 4;114,115,111,110;,
 4;116,115,114,117;,
 4;118,119,116,117;,
 4;120,121,119,118;,
 4;122,113,112,123;,
 4;122,123,121,120;,
 4;124,125,126,127;,
 4;128,129,130,131;,
 4;132,133,134,135;,
 4;136,137,138,139;,
 4;139,138,140,141;,
 4;128,127,126,129;,
 4;132,131,130,133;,
 4;134,137,136,135;,
 4;142,143,144,145;,
 4;146,147,143,142;,
 4;143,148,149,144;,
 4;145,150,151,142;,
 4;146,152,153,154;,
 4;155,156,157,147;,
 4;154,155,147,146;,
 4;146,142,151,152;,
 4;145,144,149,150;,
 4;147,157,148,143;,
 4;153,156,155,154;,
 4;158,159,160,161;,
 4;162,163,164,158;,
 4;164,165,166,167;,
 4;168,169,170,167;,
 4;96,170,171,172;,
 4;173,174,164,167;,
 4;170,96,101,175;,
 4;170,175,173,167;,
 3;176,159,158;,
 4;176,158,177,178;,
 4;164,174,177,158;,
 3;179,180,181;,
 4;103,105,179,181;,
 3;103,102,105;,
 4;177,182,183,184;,
 3;180,179,185;,
 3;177,184,178;,
 4;101,186,187,175;,
 4;175,188,189,173;,
 4;173,190,191,174;,
 4;174,192,193,177;,
 4;194,195,196,197;,
 4;198,199,200,201;,
 4;201,200,202,203;,
 4;203,202,204,205;,
 4;205,204,206,207;,
 4;207,206,208,209;,
 4;209,208,210,211;,
 4;211,210,212,213;,
 4;213,212,214,215;,
 4;215,214,216,217;,
 4;217,216,218,219;,
 4;219,218,195,194;;
 
 MeshMaterialList {
  3;
  126;
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2,
  2;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.021961;0.012549;0.605490;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.013725;0.007843;0.378431;;
  }
 }
 MeshNormals {
  139;
  0.992652;0.016119;0.119923;,
  0.016301;-0.999867;-0.000005;,
  -0.016199;0.999869;0.000017;,
  0.626583;-0.099528;0.772973;,
  0.797436;-0.442283;0.410464;,
  0.572954;-0.779179;0.254172;,
  0.438188;-0.329620;0.836267;,
  0.680977;0.011039;0.732221;,
  0.782687;0.467906;0.410446;,
  0.623026;0.119816;0.772970;,
  0.427271;0.343673;0.836259;,
  0.547410;0.797340;0.254149;,
  0.680960;0.011064;0.732237;,
  0.353944;-0.177544;-0.918260;,
  0.030224;-0.336243;-0.941290;,
  0.406050;-0.781899;-0.473029;,
  0.539367;-0.446499;-0.713948;,
  0.530925;0.008606;-0.847375;,
  0.019310;0.337070;-0.941281;,
  0.348017;0.188943;-0.918251;,
  0.524632;0.463741;-0.713936;,
  0.380500;0.794645;-0.473031;,
  0.530936;0.008626;-0.847368;,
  0.016124;-0.999870;-0.000006;,
  0.016167;-0.999869;0.000044;,
  -0.311276;-0.934019;0.175259;,
  0.304287;-0.951052;0.053939;,
  0.016411;-0.999865;-0.000011;,
  0.016347;-0.999866;-0.000038;,
  -0.034454;-0.957545;0.286216;,
  0.016224;-0.999868;0.000050;,
  0.016232;-0.999868;0.000055;,
  0.016255;-0.999868;0.000085;,
  0.016179;-0.999869;0.000026;,
  -0.154226;-0.959523;-0.235646;,
  0.016293;-0.999867;0.000076;,
  0.228516;-0.926935;0.297611;,
  0.077353;-0.929443;-0.360767;,
  0.016195;-0.999869;0.000193;,
  0.016138;-0.999870;0.000031;,
  -0.429970;-0.902655;-0.018449;,
  0.312850;-0.924010;-0.219843;,
  0.016159;-0.999869;0.000098;,
  -0.016232;0.999868;-0.000001;,
  0.016255;-0.999868;0.000011;,
  0.479078;0.870444;-0.113186;,
  -0.016208;0.999869;-0.000022;,
  0.507020;-0.854472;-0.113170;,
  0.016310;-0.999867;-0.000016;,
  -0.430533;-0.896361;-0.105723;,
  -0.339953;-0.894865;0.289222;,
  -0.583654;-0.210965;-0.784118;,
  -0.183008;-0.204513;0.961604;,
  -0.590192;0.191926;-0.784116;,
  -0.459484;0.881878;-0.105666;,
  -0.189546;0.198463;0.961605;,
  -0.368866;0.883339;0.289224;,
  -0.897781;-0.409250;-0.162800;,
  -0.736287;-0.406596;0.540889;,
  -0.609753;-0.328589;-0.721271;,
  0.155260;-0.396833;-0.904665;,
  -0.620097;0.308594;-0.721283;,
  0.142296;0.401676;-0.904659;,
  -0.910631;0.379821;-0.162748;,
  -0.710097;0.703507;-0.028980;,
  -0.416279;0.714944;-0.561754;,
  0.106060;0.723126;-0.682525;,
  0.016239;-0.999868;0.000003;,
  0.016222;-0.999868;0.000000;,
  0.016237;-0.999868;0.000007;,
  0.016224;-0.999868;0.000001;,
  0.016241;-0.999868;0.000004;,
  0.016240;-0.999868;0.000005;,
  0.016229;-0.999868;0.000007;,
  0.016226;-0.999868;0.000008;,
  -0.814752;-0.013234;0.579658;,
  -0.166293;-0.002700;0.986073;,
  0.579581;0.009411;0.814860;,
  0.985944;0.016009;0.166307;,
  0.814748;0.013232;-0.579664;,
  0.166287;0.002701;-0.986074;,
  -0.579600;-0.009416;-0.814847;,
  -0.985946;-0.016017;-0.166292;,
  0.136196;0.990540;0.016742;,
  -0.145758;0.985961;0.081455;,
  -0.038215;0.978423;0.203046;,
  0.092722;0.980549;0.172994;,
  0.113667;0.990175;-0.081447;,
  -0.168289;0.985596;-0.016727;,
  0.006436;0.979147;-0.203049;,
  -0.124502;0.977024;-0.172986;,
  -0.233701;-0.322474;0.917275;,
  0.534832;-0.390672;0.749220;,
  0.910165;-0.380142;0.164594;,
  0.747931;-0.382776;-0.542293;,
  0.897337;0.409504;0.164595;,
  0.521858;0.407849;0.749216;,
  0.735108;0.406866;-0.542288;,
  -0.244044;0.314726;0.917273;,
  -0.749105;0.382490;0.540873;,
  -0.936509;-0.227956;-0.266432;,
  -0.966498;-0.184904;0.178022;,
  -0.790426;-0.219825;0.571756;,
  -0.230559;-0.094845;0.968425;,
  0.167379;0.129419;0.977361;,
  0.557548;0.088917;0.825369;,
  0.960203;0.084358;0.266258;,
  0.957482;0.229892;-0.174293;,
  0.808634;0.116146;-0.576733;,
  0.242117;0.022921;-0.969976;,
  -0.208957;0.106648;-0.972092;,
  -0.566860;-0.061865;-0.821488;,
  -0.016274;0.999868;-0.000012;,
  -0.016239;0.999868;0.000026;,
  -0.016238;0.999868;-0.000016;,
  -0.016228;0.999868;-0.000004;,
  0.016234;-0.999868;0.000029;,
  0.016267;-0.999868;0.000025;,
  0.016248;-0.999868;0.000005;,
  0.016247;-0.999868;0.000004;,
  0.468286;0.007609;0.883544;,
  0.468286;0.007600;0.883544;,
  -0.974544;-0.015744;0.223643;,
  -0.974493;-0.015895;0.223853;,
  -0.974512;-0.015877;0.223772;,
  -0.272937;0.948346;0.161694;,
  -0.048686;0.951985;0.302249;,
  0.146211;0.955148;0.257517;,
  0.286731;0.957434;0.033242;,
  0.241996;0.956711;-0.161687;,
  0.017762;0.953066;-0.302241;,
  -0.177136;0.949905;-0.257494;,
  -0.317674;0.947618;-0.033219;,
  -0.627539;0.704832;0.330768;,
  -0.130842;0.719571;0.681981;,
  -0.974511;-0.015729;0.223787;,
  0.551794;0.722829;-0.415984;,
  0.676780;0.724861;0.128626;,
  0.391865;0.727779;0.562832;;
  126;
  4;0,0,0,0;,
  3;1,1,1;,
  3;2,2,2;,
  4;3,4,5,6;,
  3;7,4,3;,
  4;8,9,10,11;,
  3;9,8,12;,
  4;10,9,3,6;,
  4;13,14,15,16;,
  3;13,16,17;,
  4;18,19,20,21;,
  3;22,20,19;,
  4;14,13,19,18;,
  3;23,24,25;,
  3;26,27,28;,
  3;29,30,31;,
  3;32,33,34;,
  4;25,24,30,29;,
  4;29,31,35,36;,
  4;36,35,27,26;,
  4;37,38,32,34;,
  4;34,33,39,40;,
  4;41,42,38,37;,
  4;26,28,42,41;,
  4;40,39,23,25;,
  3;112,43,113;,
  3;45,43,114;,
  3;115,43,46;,
  3;114,43,115;,
  3;113,43,45;,
  3;116,44,47;,
  3;48,44,117;,
  3;117,44,116;,
  3;47,44,118;,
  3;118,44,119;,
  3;9,12,120;,
  3;121,7,3;,
  4;121,3,9,120;,
  3;47,16,15;,
  3;5,4,47;,
  3;15,5,47;,
  4;49,50,5,15;,
  4;15,14,51,49;,
  4;52,6,5,50;,
  4;14,18,53,51;,
  3;45,8,11;,
  3;21,20,45;,
  3;11,21,45;,
  4;21,54,53,18;,
  4;10,6,52,55;,
  4;21,11,56,54;,
  4;10,55,56,11;,
  3;57,122,122;,
  3;49,57,40;,
  4;50,49,40,25;,
  3;25,58,50;,
  3;123,123,58;,
  4;59,34,40,57;,
  4;60,37,34,59;,
  3;59,57,51;,
  4;59,51,53,61;,
  4;60,59,61,62;,
  4;61,63,64,65;,
  3;124,124,63;,
  4;62,61,65,66;,
  3;53,63,61;,
  4;67,67,68,68;,
  4;67,67,69,69;,
  4;70,70,68,68;,
  4;71,70,70,71;,
  4;72,72,71,71;,
  4;73,73,72,72;,
  4;74,69,69,74;,
  4;74,74,73,73;,
  4;75,75,76,76;,
  4;77,77,78,78;,
  4;79,79,80,80;,
  4;81,81,82,82;,
  4;82,82,75,75;,
  4;77,76,76,77;,
  4;79,78,78,79;,
  4;80,81,81,80;,
  4;83,84,85,86;,
  4;87,88,84,83;,
  4;84,125,126,85;,
  4;86,127,128,83;,
  4;87,129,130,89;,
  4;90,131,132,88;,
  4;89,90,88,87;,
  4;87,83,128,129;,
  4;86,85,126,127;,
  4;88,132,125,84;,
  4;130,131,90,89;,
  4;91,58,25,29;,
  4;29,36,92,91;,
  4;92,36,26,93;,
  4;26,41,94,93;,
  4;60,94,41,37;,
  4;95,96,92,93;,
  4;94,60,62,97;,
  4;94,97,95,93;,
  3;52,58,91;,
  4;52,91,98,55;,
  4;92,96,98,91;,
  3;56,99,133;,
  4;64,54,56,133;,
  3;64,63,54;,
  4;98,134,133,99;,
  3;99,135,135;,
  3;98,99,55;,
  4;62,66,136,97;,
  4;97,136,137,95;,
  4;95,137,138,96;,
  4;96,138,134,98;,
  4;100,100,101,101;,
  4;101,101,102,102;,
  4;102,102,103,103;,
  4;103,103,104,104;,
  4;104,104,105,105;,
  4;105,105,106,106;,
  4;106,106,107,107;,
  4;107,107,108,108;,
  4;108,108,109,109;,
  4;109,109,110,110;,
  4;110,110,111,111;,
  4;111,111,100,100;;
 }
 MeshTextureCoords {
  220;
  0.190840;0.921850;,
  0.229640;0.921270;,
  0.229370;0.903210;,
  0.190570;0.903800;,
  0.771610;0.284000;,
  0.810170;0.288280;,
  0.792130;0.275010;,
  0.941350;0.283830;,
  0.920800;0.274930;,
  0.902810;0.288280;,
  0.617480;0.966990;,
  0.604220;0.956670;,
  0.599710;0.962570;,
  0.603960;0.968650;,
  0.618360;0.956670;,
  0.604220;0.992030;,
  0.617480;0.981720;,
  0.603960;0.980060;,
  0.599710;0.986140;,
  0.618360;0.992030;,
  0.563270;0.966990;,
  0.577280;0.968650;,
  0.581010;0.962570;,
  0.575940;0.956670;,
  0.561800;0.956670;,
  0.577280;0.980060;,
  0.563270;0.981720;,
  0.575940;0.992030;,
  0.581010;0.986140;,
  0.561800;0.992030;,
  0.299700;0.932720;,
  0.299690;0.917520;,
  0.297480;0.916580;,
  0.346670;0.916510;,
  0.344450;0.917460;,
  0.344450;0.932710;,
  0.302770;0.911440;,
  0.303670;0.913660;,
  0.322080;0.913660;,
  0.322090;0.951780;,
  0.303680;0.951780;,
  0.302780;0.953990;,
  0.340490;0.913650;,
  0.341390;0.911440;,
  0.341390;0.953990;,
  0.340500;0.951770;,
  0.299700;0.947920;,
  0.297480;0.948860;,
  0.346670;0.948910;,
  0.344460;0.947970;,
  0.411720;0.941010;,
  0.418740;0.967220;,
  0.425760;0.941010;,
  0.437920;0.948030;,
  0.444940;0.960200;,
  0.444940;0.974240;,
  0.437920;0.986400;,
  0.474070;0.993420;,
  0.481090;0.967220;,
  0.461910;0.986400;,
  0.500270;0.986400;,
  0.488110;0.993420;,
  0.454890;0.974240;,
  0.454890;0.960200;,
  0.632500;0.992030;,
  0.632500;0.956670;,
  0.590080;0.956670;,
  0.073400;0.008630;,
  0.049810;0.008630;,
  0.049810;0.127520;,
  0.073400;0.127520;,
  0.081200;0.127520;,
  0.081200;0.008630;,
  0.149650;0.008630;,
  0.149640;0.127520;,
  0.157440;0.127520;,
  0.157440;0.008630;,
  0.095830;0.127520;,
  0.095830;0.008630;,
  0.590080;0.992030;,
  0.103630;0.127520;,
  0.103630;0.008630;,
  0.135020;0.127520;,
  0.135020;0.008630;,
  0.127220;0.127520;,
  0.127220;0.008630;,
  0.221620;0.963050;,
  0.216340;0.963050;,
  0.221630;0.968560;,
  0.221570;0.958770;,
  0.193790;0.963080;,
  0.190990;0.958810;,
  0.188510;0.963090;,
  0.188520;0.968600;,
  0.231920;0.963030;,
  0.227960;0.958760;,
  0.272320;0.963050;,
  0.271360;0.958750;,
  0.232900;0.958730;,
  0.221640;0.983180;,
  0.231950;0.988690;,
  0.272360;0.988700;,
  0.221650;0.988700;,
  0.221610;0.992970;,
  0.228010;0.992970;,
  0.216380;0.988700;,
  0.232920;0.992990;,
  0.271390;0.993000;,
  0.782500;0.158340;,
  0.769800;0.162630;,
  0.769800;0.182950;,
  0.782500;0.189910;,
  0.773640;0.149470;,
  0.766470;0.159400;,
  0.766470;0.186190;,
  0.773640;0.198780;,
  0.742070;0.198780;,
  0.742160;0.186190;,
  0.738840;0.182990;,
  0.733200;0.189910;,
  0.738840;0.162590;,
  0.733200;0.158340;,
  0.742160;0.159400;,
  0.742070;0.149470;,
  0.966720;0.285670;,
  0.990240;0.285670;,
  0.990240;0.265940;,
  0.966720;0.265940;,
  0.966720;0.216270;,
  0.990240;0.216270;,
  0.990240;0.196540;,
  0.966720;0.196540;,
  0.966720;0.146870;,
  0.990240;0.146870;,
  0.990240;0.127140;,
  0.966720;0.127140;,
  0.966720;0.077460;,
  0.990240;0.077460;,
  0.990240;0.057730;,
  0.966720;0.057730;,
  0.990240;0.008060;,
  0.966720;0.008060;,
  0.870800;0.065920;,
  0.870940;0.032960;,
  0.858600;0.045340;,
  0.858660;0.053700;,
  0.879140;0.066060;,
  0.879310;0.033070;,
  0.849550;0.009670;,
  0.835270;0.024100;,
  0.835320;0.074720;,
  0.849730;0.089100;,
  0.900440;0.089340;,
  0.914910;0.074940;,
  0.891490;0.053810;,
  0.891580;0.045410;,
  0.914910;0.023920;,
  0.900350;0.009640;,
  0.371090;0.963010;,
  0.382890;0.963020;,
  0.380810;0.958730;,
  0.373480;0.958730;,
  0.370110;0.958710;,
  0.331590;0.958720;,
  0.330630;0.963030;,
  0.328250;0.958790;,
  0.320910;0.958770;,
  0.318850;0.963020;,
  0.317520;0.958750;,
  0.285410;0.958770;,
  0.284080;0.963050;,
  0.281990;0.958760;,
  0.274720;0.958760;,
  0.318890;0.988680;,
  0.330660;0.988680;,
  0.284120;0.988710;,
  0.382880;0.968550;,
  0.371120;0.988670;,
  0.382920;0.983140;,
  0.193820;0.988730;,
  0.188550;0.988740;,
  0.191040;0.993010;,
  0.373520;0.992950;,
  0.380840;0.992960;,
  0.382920;0.988670;,
  0.188540;0.983230;,
  0.274740;0.992990;,
  0.282020;0.993000;,
  0.285450;0.992980;,
  0.317560;0.992950;,
  0.320970;0.992920;,
  0.328310;0.992940;,
  0.331630;0.992990;,
  0.370150;0.992970;,
  0.467770;0.443050;,
  0.451130;0.453860;,
  0.458670;0.463670;,
  0.475680;0.454580;,
  0.490000;0.348940;,
  0.481220;0.332480;,
  0.470320;0.338530;,
  0.479250;0.356930;,
  0.461220;0.344920;,
  0.474790;0.359420;,
  0.450170;0.354610;,
  0.461240;0.367510;,
  0.440590;0.366290;,
  0.450410;0.378670;,
  0.435690;0.377250;,
  0.448040;0.382960;,
  0.432110;0.390900;,
  0.444510;0.395480;,
  0.431210;0.405170;,
  0.444750;0.408190;,
  0.432930;0.417140;,
  0.446260;0.412870;,
  0.437710;0.431440;,
  0.453800;0.427060;,
  0.444860;0.444580;,
  0.464320;0.439250;;
 }
}