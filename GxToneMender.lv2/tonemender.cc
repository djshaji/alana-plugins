// generated from file './/tonemender.dsp' by dsp2cc:
// Code generated with Faust 0.9.73 (http://faust.grame.fr)


namespace tonemender {

class Dsp: public PluginLV2 {
private:
	uint32_t fSamplingFreq;
	double 	fConst0;
	double 	fConst1;
	double 	fConst2;
	double 	fConst3;
	double 	fConst4;
	double 	fConst5;
	FAUSTFLOAT 	fslider0;
	FAUSTFLOAT	*fslider0_;
	double 	fRec0[2];
	double 	fConst6;
	double 	fConst7;
	double 	fConst8;
	double 	fConst9;
	double 	fConst10;
	double 	fConst11;
	double 	fConst12;
	double 	fConst13;
	FAUSTFLOAT 	fslider1;
	FAUSTFLOAT	*fslider1_;
	double 	fRec1[2];
	FAUSTFLOAT 	fslider2;
	FAUSTFLOAT	*fslider2_;
	double 	fRec2[2];
	double 	fConst14;
	double 	fConst15;
	double 	fConst16;
	double 	fConst17;
	double 	fConst18;
	double 	fConst19;
	double 	fConst20;
	double 	fConst21;
	double 	fConst22;
	double 	fConst23;
	double 	fConst24;
	double 	fConst25;
	double 	fConst26;
	double 	fConst27;
	double 	fConst28;
	double 	fConst29;
	double 	fConst30;
	double 	fConst31;
	double 	fConst32;
	double 	fConst33;
	double 	fConst34;
	double 	fConst35;
	double 	fConst36;
	double 	fConst37;
	double 	fConst38;
	double 	fConst39;
	double 	fConst40;
	double 	fConst41;
	double 	fConst42;
	double 	fConst43;
	double 	fConst44;
	double 	fConst45;
	double 	fConst46;
	double 	fConst47;
	double 	fConst48;
	double 	fConst49;
	double 	fConst50;
	double 	fConst51;
	double 	fConst52;
	double 	fConst53;
	double 	fConst54;
	double 	fConst55;
	double 	fConst56;
	double 	fConst57;
	double 	fConst58;
	double 	fConst59;
	double 	fConst60;
	double 	fConst61;
	double 	fConst62;
	double 	fConst63;
	double 	fConst64;
	double 	fConst65;
	double 	fConst66;
	double 	fConst67;
	double 	fConst68;
	double 	fConst69;
	double 	fConst70;
	double 	fConst71;
	double 	fConst72;
	double 	fConst73;
	double 	fConst74;
	double 	fConst75;
	double 	fConst76;
	double 	fConst77;
	double 	fConst78;
	double 	fConst79;
	double 	fConst80;
	double 	fConst81;
	double 	fConst82;
	double 	fConst83;
	double 	fConst84;
	double 	fConst85;
	double 	fConst86;
	double 	fConst87;
	double 	fConst88;
	double 	fConst89;
	double 	fConst90;
	double 	fConst91;
	double 	fConst92;
	double 	fConst93;
	double 	fConst94;
	double 	fConst95;
	double 	fConst96;
	double 	fConst97;
	double 	fConst98;
	double 	fConst99;
	double 	fConst100;
	double 	fConst101;
	double 	fConst102;
	double 	fConst103;
	double 	fConst104;
	double 	fConst105;
	double 	fConst106;
	double 	fConst107;
	double 	fConst108;
	double 	fConst109;
	double 	fConst110;
	double 	fConst111;
	double 	fConst112;
	double 	fConst113;
	double 	fConst114;
	double 	fConst115;
	double 	fConst116;
	FAUSTFLOAT 	fslider3;
	FAUSTFLOAT	*fslider3_;
	double 	fRec4[2];
	double 	fConst117;
	double 	fConst118;
	double 	fConst119;
	double 	fConst120;
	double 	fConst121;
	double 	fConst122;
	double 	fRec5[3];
	double 	fConst123;
	double 	fRec3[6];
	double 	fConst124;
	double 	fConst125;
	double 	fConst126;
	double 	fConst127;
	double 	fConst128;
	double 	fConst129;
	double 	fConst130;
	double 	fConst131;
	double 	fConst132;
	double 	fConst133;
	double 	fConst134;
	double 	fConst135;
	double 	fConst136;
	double 	fConst137;
	double 	fConst138;
	double 	fConst139;
	double 	fConst140;
	double 	fConst141;
	double 	fConst142;
	double 	fConst143;
	double 	fConst144;
	double 	fConst145;
	double 	fConst146;
	double 	fConst147;
	double 	fConst148;
	double 	fConst149;
	double 	fConst150;
	double 	fConst151;
	double 	fConst152;
	double 	fConst153;
	double 	fConst154;
	double 	fConst155;
	double 	fConst156;
	double 	fConst157;
	double 	fConst158;
	double 	fConst159;
	double 	fConst160;
	double 	fConst161;
	double 	fConst162;
	double 	fConst163;
	double 	fConst164;
	double 	fConst165;
	double 	fConst166;
	double 	fConst167;
	double 	fConst168;
	double 	fConst169;
	double 	fConst170;
	double 	fConst171;
	double 	fConst172;
	double 	fConst173;
	double 	fConst174;
	double 	fConst175;
	double 	fConst176;
	double 	fConst177;
	double 	fConst178;
	double 	fConst179;
	double 	fConst180;
	double 	fConst181;
	double 	fConst182;
	double 	fConst183;
	double 	fConst184;
	double 	fConst185;
	double 	fConst186;
	double 	fConst187;
	double 	fConst188;
	double 	fConst189;
	double 	fConst190;
	double 	fConst191;
	double 	fConst192;
	double 	fConst193;
	double 	fConst194;
	double 	fConst195;
	double 	fConst196;
	double 	fConst197;
	double 	fConst198;
	double 	fConst199;
	double 	fConst200;
	double 	fConst201;
	double 	fConst202;
	double 	fConst203;
	double 	fConst204;
	double 	fConst205;
	double 	fConst206;
	double 	fConst207;
	double 	fConst208;
	double 	fConst209;
	double 	fConst210;
	double 	fConst211;
	double 	fConst212;
	double 	fConst213;
	double 	fConst214;
	double 	fConst215;
	double 	fConst216;
	double 	fConst217;
	double 	fConst218;
	double 	fConst219;
	double 	fConst220;
	double 	fConst221;
	double 	fConst222;
	double 	fConst223;
	double 	fConst224;
	double 	fConst225;
	double 	fConst226;
	double 	fConst227;
	double 	fConst228;
	double 	fConst229;
	double 	fConst230;
	double 	fConst231;
	double 	fConst232;
	double 	fConst233;
	double 	fConst234;
	double 	fConst235;
	double 	fConst236;
	double 	fConst237;
	double 	fConst238;
	double 	fConst239;
	double 	fConst240;
	double 	fConst241;
	double 	fConst242;
	double 	fConst243;
	double 	fConst244;
	double 	fConst245;
	double 	fConst246;
	double 	fConst247;
	double 	fConst248;
	double 	fConst249;
	double 	fConst250;
	double 	fConst251;
	double 	fConst252;
	double 	fConst253;
	double 	fConst254;
	double 	fConst255;
	double 	fConst256;
	double 	fConst257;
	double 	fConst258;
	double 	fConst259;
	double 	fConst260;
	double 	fConst261;
	double 	fConst262;
	double 	fConst263;
	double 	fConst264;
	double 	fConst265;
	double 	fConst266;
	double 	fConst267;
	double 	fConst268;
	double 	fConst269;
	double 	fConst270;
	double 	fConst271;
	double 	fConst272;
	double 	fConst273;
	double 	fConst274;
	double 	fConst275;
	double 	fConst276;
	double 	fConst277;
	double 	fConst278;
	double 	fConst279;
	double 	fConst280;
	double 	fConst281;
	double 	fConst282;
	double 	fConst283;
	double 	fConst284;
	double 	fConst285;
	double 	fConst286;
	double 	fConst287;
	double 	fConst288;
	double 	fRec6[6];
	double 	fConst289;
	double 	fConst290;
	double 	fConst291;
	double 	fConst292;
	double 	fConst293;
	double 	fConst294;
	double 	fConst295;
	double 	fConst296;
	double 	fConst297;
	double 	fConst298;
	double 	fConst299;
	double 	fConst300;
	double 	fConst301;
	double 	fConst302;
	double 	fConst303;
	double 	fConst304;
	double 	fConst305;
	double 	fConst306;
	double 	fConst307;
	double 	fConst308;
	double 	fConst309;
	double 	fConst310;
	double 	fConst311;
	double 	fConst312;
	double 	fConst313;
	double 	fConst314;
	double 	fConst315;
	double 	fConst316;
	double 	fConst317;
	double 	fConst318;
	double 	fConst319;
	double 	fConst320;
	double 	fConst321;
	double 	fConst322;
	double 	fConst323;
	double 	fConst324;
	double 	fConst325;
	double 	fConst326;
	double 	fConst327;
	double 	fConst328;
	double 	fConst329;
	double 	fConst330;
	double 	fConst331;
	double 	fConst332;
	double 	fConst333;
	double 	fConst334;
	double 	fConst335;
	double 	fConst336;
	double 	fConst337;
	FAUSTFLOAT 	fslider4;
	FAUSTFLOAT	*fslider4_;
	void connect(uint32_t port,void* data);
	void clear_state_f();
	void init(uint32_t samplingFreq);
	void compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0);

	static void clear_state_f_static(PluginLV2*);
	static void init_static(uint32_t samplingFreq, PluginLV2*);
	static void compute_static(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0, PluginLV2*);
	static void del_instance(PluginLV2 *p);
	static void connect_static(uint32_t port,void* data, PluginLV2 *p);
public:
	Dsp();
	~Dsp();
};



Dsp::Dsp()
	: PluginLV2() {
	version = PLUGINLV2_VERSION;
	id = "tonemp";
	name = N_("ToneMender");
	mono_audio = compute_static;
	stereo_audio = 0;
	set_samplerate = init_static;
	activate_plugin = 0;
	connect_ports = connect_static;
	clear_state = clear_state_f_static;
	delete_instance = del_instance;
}

Dsp::~Dsp() {
}

inline void Dsp::clear_state_f()
{
	for (int i=0; i<2; i++) fRec0[i] = 0;
	for (int i=0; i<2; i++) fRec1[i] = 0;
	for (int i=0; i<2; i++) fRec2[i] = 0;
	for (int i=0; i<2; i++) fRec4[i] = 0;
	for (int i=0; i<3; i++) fRec5[i] = 0;
	for (int i=0; i<6; i++) fRec3[i] = 0;
	for (int i=0; i<6; i++) fRec6[i] = 0;
}

void Dsp::clear_state_f_static(PluginLV2 *p)
{
	static_cast<Dsp*>(p)->clear_state_f();
}

inline void Dsp::init(uint32_t samplingFreq)
{
	fSamplingFreq = samplingFreq;
	fConst0 = double(min(192000, max(1, fSamplingFreq)));
	fConst1 = (2.15965024136301e-22 * fConst0);
	fConst2 = (fConst0 * (1.48200047109278e-18 + fConst1));
	fConst3 = (1.62098253870421e-18 + (fConst0 * (4.32409228968957e-17 + (fConst0 * (2.16463469898925e-16 + fConst2)))));
	fConst4 = (1.07982512068151e-22 * fConst0);
	fConst5 = (2.16204614484478e-17 + (fConst0 * (1.08231734949462e-16 + (fConst0 * (7.41000235546391e-19 + fConst4)))));
	fConst6 = (1.61817271707924e-24 * fConst0);
	fConst7 = (1.83296401825828e-20 + (fConst0 * (4.88900697006896e-19 + (fConst0 * (2.4464451079537e-18 + (fConst0 * (1.11215357043127e-20 + fConst6)))))));
	fConst8 = ((fConst0 * ((fConst0 * (0 - (1.62074459305014e-20 + fConst4))) - 3.2377097981779e-21)) - 1.21373623989646e-22);
	fConst9 = (fConst0 * ((fConst0 * ((fConst0 * (0 - (3.68130615470262e-22 + fConst6))) - 7.35694870489546e-23)) - 2.75824894956689e-24));
	fConst10 = (2.42725907561887e-24 * fConst0);
	fConst11 = (fConst0 * ((fConst0 * ((fConst0 * (0 - (3.6829243274197e-22 + fConst10))) - 7.35755551966437e-23)) - 2.75824894956689e-24));
	fConst12 = (1.61973768102226e-22 * fConst0);
	fConst13 = ((fConst0 * ((fConst0 * (0 - (1.62182441817082e-20 + fConst12))) - 3.23811473259816e-21)) - 1.21373623989646e-22);
	fConst14 = (2.10362453220302e-24 * fConst0);
	fConst15 = (1.83296401825828e-20 + (fConst0 * (4.88941022215298e-19 + (fConst0 * (2.4475204504853e-18 + (fConst0 * (1.64983272482333e-20 + fConst14)))))));
	fConst16 = (1.40377265688596e-22 * fConst0);
	fConst17 = (2.16231551426347e-17 + (fConst0 * (1.08303567037408e-16 + (fConst0 * (1.10016593941944e-18 + fConst16)))));
	fConst18 = (3.88737043445343e-22 * fConst0);
	fConst19 = (1.78308079257463e-18 + (fConst0 * (4.75704260494182e-17 + (fConst0 * (2.3825410769818e-16 + (fConst0 * (2.35167913103613e-18 + fConst18)))))));
	fConst20 = (5.82542178148528e-24 * fConst0);
	fConst21 = (8.10491269352103e-19 + (fConst0 * (2.16600609048209e-17 + (fConst0 * (1.09288047895759e-16 + (fConst0 * (6.02866758246929e-18 + (fConst0 * (3.58320259719234e-20 + fConst20)))))))));
	fConst22 = (3.23634543415849e-24 * fConst0);
	fConst23 = (8.10491269352103e-19 + (fConst0 * (2.16573554734044e-17 + (fConst0 * (1.09215799783016e-16 + (fConst0 * (5.66527955924445e-18 + (fConst0 * (2.27829839689661e-20 + fConst22)))))))));
	fConst24 = (7.55877584477055e-24 * fConst0);
	fConst25 = (fConst0 * (3.60731747641189e-20 - fConst24));
	fConst26 = (fConst25 - 3.60724188865344e-21);
	fConst27 = (4.04543179269811e-26 * fConst0);
	fConst28 = (fConst0 * (4.04543179269811e-27 - fConst27));
	fConst29 = (2.69956280170377e-24 * fConst0);
	fConst30 = (fConst29 - 2.69956280170377e-25);
	fConst31 = (fConst0 * (2.68834722677882e-23 + (fConst0 * ((fConst0 * (7.03905131929471e-25 - fConst22)) - 2.68864658873147e-22))));
	fConst32 = ((fConst0 * (1.96031710627571e-20 + (fConst0 * (fConst1 - 7.87179210783303e-21)))) - 1.86230709218985e-21);
	fConst33 = (5.83844822835416e-22 * fConst0);
	fConst34 = ((fConst0 * (5.10015409360202e-19 + (fConst0 * (fConst33 - 2.46026331316586e-18)))) - 2.02506334226358e-20);
	fConst35 = (2.26544180391094e-24 * fConst0);
	fConst36 = ((fConst0 * (5.10101987338767e-19 + (fConst0 * ((fConst0 * (1.14065565590132e-20 - fConst35)) - 2.46248189199011e-18)))) - 2.02506334226358e-20);
	fConst37 = (1.51175516895411e-22 * fConst0);
	fConst38 = ((fConst0 * (1.83971301603045e-19 + (fConst0 * (fConst37 - 7.21486927487496e-19)))) - 9.37882891049895e-21);
	fConst39 = (fConst0 * (fConst1 - 1.48200047109278e-18));
	fConst40 = (1.62098253870421e-18 + (fConst0 * ((fConst0 * (2.16463469898925e-16 + fConst39)) - 4.32409228968957e-17)));
	fConst41 = ((fConst0 * (1.08231734949462e-16 + (fConst0 * (fConst4 - 7.41000235546391e-19)))) - 2.16204614484478e-17);
	fConst42 = ((fConst0 * (4.88900697006896e-19 + (fConst0 * ((fConst0 * (1.11215357043127e-20 - fConst6)) - 2.4464451079537e-18)))) - 1.83296401825828e-20);
	fConst43 = (1.21373623989646e-22 + (fConst0 * ((fConst0 * (1.62074459305014e-20 - fConst4)) - 3.2377097981779e-21)));
	fConst44 = (fConst0 * ((fConst0 * (7.35694870489546e-23 + (fConst0 * (fConst6 - 3.68130615470262e-22)))) - 2.75824894956689e-24));
	fConst45 = (fConst0 * ((fConst0 * (7.35755551966437e-23 + (fConst0 * (fConst10 - 3.6829243274197e-22)))) - 2.75824894956689e-24));
	fConst46 = (1.21373623989646e-22 + (fConst0 * ((fConst0 * (1.62182441817082e-20 - fConst12)) - 3.23811473259816e-21)));
	fConst47 = ((fConst0 * (4.88941022215298e-19 + (fConst0 * ((fConst0 * (1.64983272482333e-20 - fConst14)) - 2.4475204504853e-18)))) - 1.83296401825828e-20);
	fConst48 = ((fConst0 * (1.08303567037408e-16 + (fConst0 * (fConst16 - 1.10016593941944e-18)))) - 2.16231551426347e-17);
	fConst49 = (1.78308079257463e-18 + (fConst0 * ((fConst0 * (2.3825410769818e-16 + (fConst0 * (fConst18 - 2.35167913103613e-18)))) - 4.75704260494182e-17)));
	fConst50 = (8.10491269352103e-19 + (fConst0 * ((fConst0 * (1.09288047895759e-16 + (fConst0 * ((fConst0 * (3.58320259719234e-20 - fConst20)) - 6.02866758246929e-18)))) - 2.16600609048209e-17)));
	fConst51 = (8.10491269352103e-19 + (fConst0 * ((fConst0 * (1.09215799783016e-16 + (fConst0 * ((fConst0 * (2.27829839689661e-20 - fConst22)) - 5.66527955924445e-18)))) - 2.16573554734044e-17)));
	fConst52 = (6.47895072408904e-22 * fConst0);
	fConst53 = (8.10491269352103e-18 + (fConst0 * ((fConst0 * (2.16463469898925e-16 + (fConst0 * (1.48200047109278e-18 - fConst52)))) - 1.29722768690687e-16)));
	fConst54 = (3.23947536204452e-22 * fConst0);
	fConst55 = ((fConst0 * (1.08231734949462e-16 + (fConst0 * (7.41000235546391e-19 - fConst54)))) - 6.48613843453435e-17);
	fConst56 = (8.09086358539622e-24 * fConst0);
	fConst57 = ((fConst0 * (4.88900697006896e-19 + (fConst0 * (2.4464451079537e-18 + (fConst0 * (fConst56 - 3.33646071129381e-20)))))) - 5.49889205477485e-20);
	fConst58 = (3.64120871968937e-22 + (fConst0 * ((fConst0 * (fConst54 - 1.62074459305014e-20)) - 3.2377097981779e-21)));
	fConst59 = (fConst0 * ((fConst0 * ((fConst0 * (1.10439184641079e-21 - fConst56)) - 7.35694870489546e-23)) - 2.75824894956689e-24));
	fConst60 = (1.21362953780943e-23 * fConst0);
	fConst61 = (fConst0 * ((fConst0 * ((fConst0 * (1.10487729822591e-21 - fConst60)) - 7.35755551966437e-23)) - 2.75824894956689e-24));
	fConst62 = (4.85921304306678e-22 * fConst0);
	fConst63 = (3.64120871968937e-22 + (fConst0 * ((fConst0 * (fConst62 - 1.62182441817082e-20)) - 3.23811473259816e-21)));
	fConst64 = (1.05181226610151e-23 * fConst0);
	fConst65 = ((fConst0 * (4.88941022215298e-19 + (fConst0 * (2.4475204504853e-18 + (fConst0 * (fConst64 - 4.94949817447e-20)))))) - 5.49889205477485e-20);
	fConst66 = (4.21131797065788e-22 * fConst0);
	fConst67 = ((fConst0 * (1.08303567037408e-16 + (fConst0 * (1.10016593941944e-18 - fConst66)))) - 6.48694654279042e-17);
	fConst68 = (1.16621113033603e-21 * fConst0);
	fConst69 = (8.91540396287313e-18 + (fConst0 * ((fConst0 * (2.3825410769818e-16 + (fConst0 * (2.35167913103613e-18 - fConst68)))) - 1.42711278148255e-16)));
	fConst70 = (2.91271089074264e-23 * fConst0);
	fConst71 = (4.05245634676051e-18 + (fConst0 * ((fConst0 * (1.09288047895759e-16 + (fConst0 * (6.02866758246929e-18 + (fConst0 * (fConst70 - 1.0749607791577e-19)))))) - 6.49801827144628e-17)));
	fConst72 = (1.61817271707924e-23 * fConst0);
	fConst73 = (4.05245634676051e-18 + (fConst0 * ((fConst0 * (1.09215799783016e-16 + (fConst0 * (5.66527955924445e-18 + (fConst0 * (fConst72 - 6.83489519068984e-20)))))) - 6.49720664202133e-17)));
	fConst74 = (4.31930048272603e-22 * fConst0);
	fConst75 = (1.62098253870421e-17 + (fConst0 * ((fConst0 * ((fConst0 * (2.96400094218557e-18 + fConst74)) - 4.3292693979785e-16)) - 8.64818457937913e-17)));
	fConst76 = ((fConst0 * (fConst2 - 2.16463469898925e-16)) - 4.32409228968957e-17);
	fConst77 = ((fConst0 * ((fConst0 * (4.8928902159074e-18 + (fConst0 * (2.22430714086254e-20 - fConst72)))) - 9.77801394013792e-19)) - 3.66592803651657e-20);
	fConst78 = (2.42747247979291e-22 + (fConst0 * (6.4754195963558e-21 + (fConst0 * (0 - (3.24148918610028e-20 + fConst1))))));
	fConst79 = (fConst0 * (5.51649789913379e-24 + (fConst0 * ((fConst0 * (fConst72 - 7.36261230940524e-22)) - 1.47138974097909e-22))));
	fConst80 = (2.42725907561887e-23 * fConst0);
	fConst81 = (fConst0 * (5.51649789913379e-24 + (fConst0 * ((fConst0 * (fConst80 - 7.3658486548394e-22)) - 1.47151110393287e-22))));
	fConst82 = (2.42747247979291e-22 + (fConst0 * (6.47622946519631e-21 + (fConst0 * (0 - (3.24364883634165e-20 + fConst54))))));
	fConst83 = (2.10362453220302e-23 * fConst0);
	fConst84 = ((fConst0 * ((fConst0 * (4.8950409009706e-18 + (fConst0 * (3.29966544964667e-20 - fConst83)))) - 9.77882044430596e-19)) - 3.66592803651657e-20);
	fConst85 = (2.80754531377192e-22 * fConst0);
	fConst86 = ((fConst0 * ((fConst0 * (2.20033187883889e-18 + fConst85)) - 2.16607134074815e-16)) - 4.32463102852695e-17);
	fConst87 = (7.77474086890685e-22 * fConst0);
	fConst88 = (1.78308079257463e-17 + (fConst0 * ((fConst0 * ((fConst0 * (4.70335826207226e-18 + fConst87)) - 4.7650821539636e-16)) - 9.51408520988364e-17)));
	fConst89 = (5.82542178148528e-23 * fConst0);
	fConst90 = (8.10491269352103e-18 + (fConst0 * ((fConst0 * ((fConst0 * (1.20573351649386e-17 + (fConst0 * (7.16640519438468e-20 - fConst89)))) - 2.18576095791518e-16)) - 4.33201218096419e-17)));
	fConst91 = (3.23634543415849e-23 * fConst0);
	fConst92 = (8.10491269352103e-18 + (fConst0 * ((fConst0 * ((fConst0 * (1.13305591184889e-17 + (fConst0 * (4.55659679379323e-20 - fConst91)))) - 2.18431599566032e-16)) - 4.33147109468089e-17)));
	fConst93 = (1.62098253870421e-17 + (fConst0 * (8.64818457937913e-17 + (fConst0 * ((fConst0 * (fConst74 - 2.96400094218557e-18)) - 4.3292693979785e-16)))));
	fConst94 = (4.32409228968957e-17 + (fConst0 * (fConst39 - 2.16463469898925e-16)));
	fConst95 = (3.66592803651657e-20 + (fConst0 * ((fConst0 * ((fConst0 * (2.22430714086254e-20 + fConst72)) - 4.8928902159074e-18)) - 9.77801394013792e-19)));
	fConst96 = ((fConst0 * (6.4754195963558e-21 + (fConst0 * (3.24148918610028e-20 - fConst1)))) - 2.42747247979291e-22);
	fConst97 = (fConst0 * (5.51649789913379e-24 + (fConst0 * (1.47138974097909e-22 + (fConst0 * (0 - (7.36261230940524e-22 + fConst72)))))));
	fConst98 = (fConst0 * (5.51649789913379e-24 + (fConst0 * (1.47151110393287e-22 + (fConst0 * (0 - (7.3658486548394e-22 + fConst80)))))));
	fConst99 = ((fConst0 * (6.47622946519631e-21 + (fConst0 * (3.24364883634165e-20 - fConst54)))) - 2.42747247979291e-22);
	fConst100 = (3.66592803651657e-20 + (fConst0 * ((fConst0 * ((fConst0 * (3.29966544964667e-20 + fConst83)) - 4.8950409009706e-18)) - 9.77882044430596e-19)));
	fConst101 = (4.32463102852695e-17 + (fConst0 * ((fConst0 * (fConst85 - 2.20033187883889e-18)) - 2.16607134074815e-16)));
	fConst102 = (1.78308079257463e-17 + (fConst0 * (9.51408520988364e-17 + (fConst0 * ((fConst0 * (fConst87 - 4.70335826207226e-18)) - 4.7650821539636e-16)))));
	fConst103 = (8.10491269352103e-18 + (fConst0 * (4.33201218096419e-17 + (fConst0 * ((fConst0 * ((fConst0 * (7.16640519438468e-20 + fConst89)) - 1.20573351649386e-17)) - 2.18576095791518e-16)))));
	fConst104 = (8.10491269352103e-18 + (fConst0 * (4.33147109468089e-17 + (fConst0 * ((fConst0 * ((fConst0 * (4.55659679379323e-20 + fConst91)) - 1.13305591184889e-17)) - 2.18431599566032e-16)))));
	fConst105 = (8.10491269352103e-18 + (fConst0 * (1.29722768690687e-16 + (fConst0 * (2.16463469898925e-16 + (fConst0 * (0 - (1.48200047109278e-18 + fConst52))))))));
	fConst106 = (6.48613843453435e-17 + (fConst0 * (1.08231734949462e-16 + (fConst0 * (0 - (7.41000235546391e-19 + fConst54))))));
	fConst107 = (5.49889205477485e-20 + (fConst0 * (4.88900697006896e-19 + (fConst0 * ((fConst0 * (0 - (3.33646071129381e-20 + fConst56))) - 2.4464451079537e-18)))));
	fConst108 = ((fConst0 * ((fConst0 * (1.62074459305014e-20 + fConst54)) - 3.2377097981779e-21)) - 3.64120871968937e-22);
	fConst109 = (fConst0 * ((fConst0 * (7.35694870489546e-23 + (fConst0 * (1.10439184641079e-21 + fConst56)))) - 2.75824894956689e-24));
	fConst110 = (fConst0 * ((fConst0 * (7.35755551966437e-23 + (fConst0 * (1.10487729822591e-21 + fConst60)))) - 2.75824894956689e-24));
	fConst111 = ((fConst0 * ((fConst0 * (1.62182441817082e-20 + fConst62)) - 3.23811473259816e-21)) - 3.64120871968937e-22);
	fConst112 = (5.49889205477485e-20 + (fConst0 * (4.88941022215298e-19 + (fConst0 * ((fConst0 * (0 - (4.94949817447e-20 + fConst64))) - 2.4475204504853e-18)))));
	fConst113 = (6.48694654279042e-17 + (fConst0 * (1.08303567037408e-16 + (fConst0 * (0 - (1.10016593941944e-18 + fConst66))))));
	fConst114 = (8.91540396287313e-18 + (fConst0 * (1.42711278148255e-16 + (fConst0 * (2.3825410769818e-16 + (fConst0 * (0 - (2.35167913103613e-18 + fConst68))))))));
	fConst115 = (4.05245634676051e-18 + (fConst0 * (6.49801827144628e-17 + (fConst0 * (1.09288047895759e-16 + (fConst0 * ((fConst0 * (0 - (1.0749607791577e-19 + fConst70))) - 6.02866758246929e-18)))))));
	fConst116 = (4.05245634676051e-18 + (fConst0 * (6.49720664202133e-17 + (fConst0 * (1.09215799783016e-16 + (fConst0 * ((fConst0 * (0 - (6.83489519068984e-20 + fConst72))) - 5.66527955924445e-18)))))));
	fConst117 = (4.33054407650898e-10 * fConst0);
	fConst118 = (1.22474303201741e-06 + (fConst0 * (fConst117 - 4.66962506103765e-08)));
	fConst119 = faustpower<2>(fConst0);
	fConst120 = (2.44948606403482e-06 - (8.66108815301797e-10 * fConst119));
	fConst121 = (1.22474303201741e-06 + (fConst0 * (4.66962506103765e-08 + fConst117)));
	fConst122 = (1.0 / fConst121);
	fConst123 = (fConst119 / fConst121);
	fConst124 = (fConst0 * (3.60731747641189e-20 + fConst24));
	fConst125 = (fConst124 - 1.08217256659603e-20);
	fConst126 = (1.21362953780943e-25 * fConst0);
	fConst127 = (fConst0 * (fConst126 - 4.04543179269811e-27));
	fConst128 = (0 - (2.69956280170377e-25 + fConst29));
	fConst129 = (fConst0 * (2.68834722677882e-23 + (fConst0 * (2.68864658873147e-22 + (fConst0 * (fConst72 - 2.11171539578841e-24))))));
	fConst130 = ((fConst0 * (1.96031710627571e-20 + (fConst0 * (7.87179210783303e-21 - fConst52)))) - 5.58692127656954e-21);
	fConst131 = (1.75153446850625e-21 * fConst0);
	fConst132 = ((fConst0 * (5.10015409360202e-19 + (fConst0 * (2.46026331316586e-18 - fConst131)))) - 6.07519002679074e-20);
	fConst133 = (1.13272090195547e-23 * fConst0);
	fConst134 = ((fConst0 * (5.10101987338767e-19 + (fConst0 * (2.46248189199011e-18 + (fConst0 * (fConst133 - 3.42196696770396e-20)))))) - 6.07519002679074e-20);
	fConst135 = (4.53526550686233e-22 * fConst0);
	fConst136 = ((fConst0 * (1.83971301603045e-19 + (fConst0 * (7.21486927487496e-19 - fConst135)))) - 2.81364867314968e-20);
	fConst137 = (8.09086358539622e-26 * fConst0);
	fConst138 = (fConst0 * (0 - (8.09086358539622e-27 + fConst137)));
	fConst139 = (5.39912560340754e-24 * fConst0);
	fConst140 = (5.39912560340754e-25 - fConst139);
	fConst141 = (fConst0 * ((fConst0 * (5.37729317746295e-22 + (fConst0 * (1.40781026385894e-24 - fConst91)))) - 5.37669445355763e-23));
	fConst142 = ((fConst0 * ((fConst0 * (1.57435842156661e-20 + fConst74)) - 3.92063421255143e-20)) - 3.72461418437969e-21);
	fConst143 = (1.16768964567083e-21 * fConst0);
	fConst144 = ((fConst0 * ((fConst0 * (4.92052662633171e-18 + fConst143)) - 1.0200308187204e-18)) - 4.05012668452716e-20);
	fConst145 = (2.26544180391094e-23 * fConst0);
	fConst146 = ((fConst0 * ((fConst0 * (4.92496378398023e-18 + (fConst0 * (2.28131131180264e-20 - fConst145)))) - 1.02020397467753e-18)) - 4.05012668452716e-20);
	fConst147 = (3.02351033790822e-22 * fConst0);
	fConst148 = ((fConst0 * ((fConst0 * (1.44297385497499e-18 + fConst147)) - 3.6794260320609e-19)) - 1.87576578209979e-20);
	fConst149 = (1.51175516895411e-23 * fConst0);
	fConst150 = ((fConst0 * (fConst149 - 7.21463495282378e-20)) - 7.21448377730688e-21);
	fConst151 = (7.21448377730688e-21 + (fConst0 * (0 - (7.21463495282378e-20 + fConst149))));
	fConst152 = (fConst0 * (8.09086358539622e-27 - fConst137));
	fConst153 = (5.39912560340754e-25 + fConst139);
	fConst154 = (fConst0 * ((fConst0 * ((fConst0 * (1.40781026385894e-24 + fConst91)) - 5.37729317746295e-22)) - 5.37669445355763e-23));
	fConst155 = (3.72461418437969e-21 + (fConst0 * ((fConst0 * (fConst74 - 1.57435842156661e-20)) - 3.92063421255143e-20)));
	fConst156 = (4.05012668452716e-20 + (fConst0 * ((fConst0 * (fConst143 - 4.92052662633171e-18)) - 1.0200308187204e-18)));
	fConst157 = (4.05012668452716e-20 + (fConst0 * ((fConst0 * ((fConst0 * (2.28131131180264e-20 + fConst145)) - 4.92496378398023e-18)) - 1.02020397467753e-18)));
	fConst158 = (1.87576578209979e-20 + (fConst0 * ((fConst0 * (fConst147 - 1.44297385497499e-18)) - 3.6794260320609e-19)));
	fConst159 = (1.08217256659603e-20 + fConst25);
	fConst160 = (fConst0 * (4.04543179269811e-27 + fConst126));
	fConst161 = (fConst0 * (2.68834722677882e-23 + (fConst0 * ((fConst0 * (0 - (2.11171539578841e-24 + fConst72))) - 2.68864658873147e-22))));
	fConst162 = (5.58692127656954e-21 + (fConst0 * (1.96031710627571e-20 + (fConst0 * (0 - (7.87179210783303e-21 + fConst52))))));
	fConst163 = (6.07519002679074e-20 + (fConst0 * (5.10015409360202e-19 + (fConst0 * (0 - (2.46026331316586e-18 + fConst131))))));
	fConst164 = (6.07519002679074e-20 + (fConst0 * (5.10101987338767e-19 + (fConst0 * ((fConst0 * (0 - (3.42196696770396e-20 + fConst133))) - 2.46248189199011e-18)))));
	fConst165 = (2.81364867314968e-20 + (fConst0 * (1.83971301603045e-19 + (fConst0 * (0 - (7.21486927487496e-19 + fConst135))))));
	fConst166 = (3.60724188865344e-21 + fConst124);
	fConst167 = (fConst0 * (0 - (4.04543179269811e-27 + fConst27)));
	fConst168 = (fConst0 * (2.68834722677882e-23 + (fConst0 * (2.68864658873147e-22 + (fConst0 * (7.03905131929471e-25 + fConst22))))));
	fConst169 = (1.86230709218985e-21 + (fConst0 * (1.96031710627571e-20 + (fConst0 * (7.87179210783303e-21 + fConst1)))));
	fConst170 = (2.02506334226358e-20 + (fConst0 * (5.10015409360202e-19 + (fConst0 * (2.46026331316586e-18 + fConst33)))));
	fConst171 = (2.02506334226358e-20 + (fConst0 * (5.10101987338767e-19 + (fConst0 * (2.46248189199011e-18 + (fConst0 * (1.14065565590132e-20 + fConst35)))))));
	fConst172 = (9.37882891049895e-21 + (fConst0 * (1.83971301603045e-19 + (fConst0 * (7.21486927487496e-19 + fConst37)))));
	fConst173 = (2.87766735657873e-22 * fConst0);
	fConst174 = (fConst0 * (3.2842746636103e-18 + fConst173));
	fConst175 = (3.75883496153286e-18 + (fConst0 * (1.00268308666246e-16 + (fConst0 * (5.01915620836652e-16 + fConst174)))));
	fConst176 = (1.43883367828937e-22 * fConst0);
	fConst177 = (5.01341543331231e-17 + (fConst0 * (2.50957810418326e-16 + (fConst0 * (1.64213733180515e-18 + fConst176)))));
	fConst178 = (1.48281131895469e-24 * fConst0);
	fConst179 = (3.77134342405142e-20 + (fConst0 * (1.00586055188086e-18 + (fConst0 * (5.03227242565336e-18 + (fConst0 * (1.69986626134739e-20 + fConst178)))))));
	fConst180 = ((fConst0 * ((fConst0 * (0 - (2.23579793820793e-20 + fConst176))) - 4.46655995854184e-21)) - 1.67442042182383e-22);
	fConst181 = (fConst0 * ((fConst0 * ((fConst0 * (0 - (4.49670395866367e-22 + fConst178))) - 8.988218077711e-23)) - 3.37002572489702e-24));
	fConst182 = (2.22421697843203e-24 * fConst0);
	fConst183 = (fConst0 * ((fConst0 * ((fConst0 * (0 - (4.49818676998263e-22 + fConst182))) - 8.98877413195561e-23)) - 3.37002572489702e-24));
	fConst184 = (2.15825051743405e-22 * fConst0);
	fConst185 = ((fConst0 * ((fConst0 * (0 - (2.23723677188622e-20 + fConst184))) - 4.4670995211712e-21)) - 1.67442042182383e-22);
	fConst186 = (1.92765471464109e-24 * fConst0);
	fConst187 = (3.77134342405142e-20 + (fConst0 * (1.00592277904735e-18 + (fConst0 * (5.03393182009627e-18 + (fConst0 * (2.52957071150661e-20 + fConst186)))))));
	fConst188 = (1.87048378177617e-22 * fConst0);
	fConst189 = (5.01401950118907e-17 + (fConst0 * (2.51118895509197e-16 + (fConst0 * (2.44756980047705e-18 + fConst188)))));
	fConst190 = (5.17980124184172e-22 * fConst0);
	fConst191 = (4.13471845768615e-18 + (fConst0 * (1.10307256741743e-16 + (fConst0 * (5.52430310001027e-16 + (fConst0 * (5.22837026698717e-18 + fConst190)))))));
	fConst192 = (5.33812074823688e-24 * fConst0);
	fConst193 = (1.87941748076643e-18 + (fConst0 * (5.02140337747609e-17 + (fConst0 * (2.53088430104688e-16 + (fConst0 * (1.23045691807846e-17 + (fConst0 * (5.47622833624311e-20 + fConst192)))))))));
	fConst194 = (2.96562263790938e-24 * fConst0);
	fConst195 = (1.87941748076643e-18 + (fConst0 * (5.02098680123042e-17 + (fConst0 * (2.52977184439414e-16 + (fConst0 * (1.17450418713893e-17 + (fConst0 * (3.47167420660924e-20 + fConst194)))))))));
	fConst196 = (1.00718357480256e-23 * fConst0);
	fConst197 = (fConst0 * (8.07823996672174e-20 - fConst196));
	fConst198 = (fConst197 - 8.07813924836426e-21);
	fConst199 = (3.70702829738672e-26 * fConst0);
	fConst200 = (fConst0 * (3.70702829738672e-27 - fConst199));
	fConst201 = (3.59708419572341e-24 * fConst0);
	fConst202 = (fConst201 - 3.59708419572341e-25);
	fConst203 = (fConst0 * (4.14847776645656e-23 + (fConst0 * ((fConst0 * (6.45022923745289e-25 - fConst194)) - 4.14875208655057e-22))));
	fConst204 = ((fConst0 * (4.27834472507198e-20 + (fConst0 * (fConst173 - 1.20130350979737e-20)))) - 4.12869796792939e-21);
	fConst205 = (8.49541486599703e-22 * fConst0);
	fConst206 = ((fConst0 * (1.30237550868038e-18 + (fConst0 * (fConst205 - 6.31048104939148e-18)))) - 5.13601191123194e-20);
	fConst207 = (2.07593584653656e-24 * fConst0);
	fConst208 = ((fConst0 * (1.30250876475573e-18 + (fConst0 * ((fConst0 * (1.75069764732726e-20 - fConst207)) - 6.31389575293003e-18)))) - 5.13601191123194e-20);
	fConst209 = (2.01436714960511e-22 * fConst0);
	fConst210 = ((fConst0 * (4.11987720343872e-19 + (fConst0 * (fConst209 - 1.61567921603517e-18)))) - 2.10031620457471e-20);
	fConst211 = (fConst0 * (fConst173 - 3.2842746636103e-18));
	fConst212 = (3.75883496153286e-18 + (fConst0 * ((fConst0 * (5.01915620836652e-16 + fConst211)) - 1.00268308666246e-16)));
	fConst213 = ((fConst0 * (2.50957810418326e-16 + (fConst0 * (fConst176 - 1.64213733180515e-18)))) - 5.01341543331231e-17);
	fConst214 = ((fConst0 * (1.00586055188086e-18 + (fConst0 * ((fConst0 * (1.69986626134739e-20 - fConst178)) - 5.03227242565336e-18)))) - 3.77134342405142e-20);
	fConst215 = (1.67442042182383e-22 + (fConst0 * ((fConst0 * (2.23579793820793e-20 - fConst176)) - 4.46655995854184e-21)));
	fConst216 = (fConst0 * ((fConst0 * (8.988218077711e-23 + (fConst0 * (fConst178 - 4.49670395866367e-22)))) - 3.37002572489702e-24));
	fConst217 = (fConst0 * ((fConst0 * (8.98877413195561e-23 + (fConst0 * (fConst182 - 4.49818676998263e-22)))) - 3.37002572489702e-24));
	fConst218 = (1.67442042182383e-22 + (fConst0 * ((fConst0 * (2.23723677188622e-20 - fConst184)) - 4.4670995211712e-21)));
	fConst219 = ((fConst0 * (1.00592277904735e-18 + (fConst0 * ((fConst0 * (2.52957071150661e-20 - fConst186)) - 5.03393182009627e-18)))) - 3.77134342405142e-20);
	fConst220 = ((fConst0 * (2.51118895509197e-16 + (fConst0 * (fConst188 - 2.44756980047705e-18)))) - 5.01401950118907e-17);
	fConst221 = (4.13471845768615e-18 + (fConst0 * ((fConst0 * (5.52430310001027e-16 + (fConst0 * (fConst190 - 5.22837026698717e-18)))) - 1.10307256741743e-16)));
	fConst222 = (1.87941748076643e-18 + (fConst0 * ((fConst0 * (2.53088430104688e-16 + (fConst0 * ((fConst0 * (5.47622833624311e-20 - fConst192)) - 1.23045691807846e-17)))) - 5.02140337747609e-17)));
	fConst223 = (1.87941748076643e-18 + (fConst0 * ((fConst0 * (2.52977184439414e-16 + (fConst0 * ((fConst0 * (3.47167420660924e-20 - fConst194)) - 1.17450418713893e-17)))) - 5.02098680123042e-17)));
	fConst224 = (8.63300206973619e-22 * fConst0);
	fConst225 = (1.87941748076643e-17 + (fConst0 * ((fConst0 * (5.01915620836652e-16 + (fConst0 * (3.2842746636103e-18 - fConst224)))) - 3.00804925998739e-16)));
	fConst226 = (4.3165010348681e-22 * fConst0);
	fConst227 = ((fConst0 * (2.50957810418326e-16 + (fConst0 * (1.64213733180515e-18 - fConst226)))) - 1.50402462999369e-16);
	fConst228 = (7.41405659477344e-24 * fConst0);
	fConst229 = ((fConst0 * (1.00586055188086e-18 + (fConst0 * (5.03227242565336e-18 + (fConst0 * (fConst228 - 5.09959878404216e-20)))))) - 1.13140302721543e-19);
	fConst230 = (5.0232612654715e-22 + (fConst0 * ((fConst0 * (fConst226 - 2.23579793820793e-20)) - 4.46655995854184e-21)));
	fConst231 = (fConst0 * ((fConst0 * ((fConst0 * (1.3490111875991e-21 - fConst228)) - 8.988218077711e-23)) - 3.37002572489702e-24));
	fConst232 = (1.11210848921602e-23 * fConst0);
	fConst233 = (fConst0 * ((fConst0 * ((fConst0 * (1.34945603099479e-21 - fConst232)) - 8.98877413195561e-23)) - 3.37002572489702e-24));
	fConst234 = (6.47475155230214e-22 * fConst0);
	fConst235 = (5.0232612654715e-22 + (fConst0 * ((fConst0 * (fConst234 - 2.23723677188622e-20)) - 4.4670995211712e-21)));
	fConst236 = (9.63827357320547e-24 * fConst0);
	fConst237 = ((fConst0 * (1.00592277904735e-18 + (fConst0 * (5.03393182009627e-18 + (fConst0 * (fConst236 - 7.58871213451984e-20)))))) - 1.13140302721543e-19);
	fConst238 = (5.61145134532852e-22 * fConst0);
	fConst239 = ((fConst0 * (2.51118895509197e-16 + (fConst0 * (2.44756980047705e-18 - fConst238)))) - 1.50420585035672e-16);
	fConst240 = (1.55394037255251e-21 * fConst0);
	fConst241 = (2.06735922884308e-17 + (fConst0 * ((fConst0 * (5.52430310001027e-16 + (fConst0 * (5.22837026698717e-18 - fConst240)))) - 3.3092177022523e-16)));
	fConst242 = (2.66906037411844e-23 * fConst0);
	fConst243 = (9.39708740383216e-18 + (fConst0 * ((fConst0 * (2.53088430104688e-16 + (fConst0 * (1.23045691807846e-17 + (fConst0 * (fConst242 - 1.64286850087293e-19)))))) - 1.50642101324283e-16)));
	fConst244 = (1.48281131895469e-23 * fConst0);
	fConst245 = (9.39708740383216e-18 + (fConst0 * ((fConst0 * (2.52977184439414e-16 + (fConst0 * (1.17450418713893e-17 + (fConst0 * (fConst244 - 1.04150226198277e-19)))))) - 1.50629604036912e-16)));
	fConst246 = (5.75533471315746e-22 * fConst0);
	fConst247 = (3.75883496153286e-17 + (fConst0 * ((fConst0 * ((fConst0 * (6.5685493272206e-18 + fConst246)) - 1.0038312416733e-15)) - 2.00536617332493e-16)));
	fConst248 = ((fConst0 * (fConst174 - 5.01915620836652e-16)) - 1.00268308666246e-16);
	fConst249 = ((fConst0 * ((fConst0 * (1.00645448513067e-17 + (fConst0 * (3.39973252269477e-20 - fConst244)))) - 2.01172110376171e-18)) - 7.54268684810284e-20);
	fConst250 = (3.34884084364767e-22 + (fConst0 * (8.93311991708369e-21 + (fConst0 * (0 - (4.47159587641586e-20 + fConst173))))));
	fConst251 = (fConst0 * (6.74005144979404e-24 + (fConst0 * ((fConst0 * (fConst244 - 8.99340791732735e-22)) - 1.7976436155422e-22))));
	fConst252 = (2.22421697843203e-23 * fConst0);
	fConst253 = (fConst0 * (6.74005144979404e-24 + (fConst0 * ((fConst0 * (fConst252 - 8.99637353996526e-22)) - 1.79775482639112e-22))));
	fConst254 = (3.34884084364767e-22 + (fConst0 * (8.93419904234241e-21 + (fConst0 * (0 - (4.47447354377244e-20 + fConst226))))));
	fConst255 = (1.92765471464109e-23 * fConst0);
	fConst256 = ((fConst0 * ((fConst0 * (1.00678636401925e-17 + (fConst0 * (5.05914142301322e-20 - fConst255)))) - 2.01184555809471e-18)) - 7.54268684810284e-20);
	fConst257 = (3.74096756355235e-22 * fConst0);
	fConst258 = ((fConst0 * ((fConst0 * (4.8951396009541e-18 + fConst257)) - 5.02237791018395e-16)) - 1.00280390023781e-16);
	fConst259 = (1.03596024836834e-21 * fConst0);
	fConst260 = (4.13471845768615e-17 + (fConst0 * ((fConst0 * ((fConst0 * (1.04567405339743e-17 + fConst259)) - 1.10486062000205e-15)) - 2.20614513483487e-16)));
	fConst261 = (5.33812074823688e-23 * fConst0);
	fConst262 = (1.87941748076643e-17 + (fConst0 * ((fConst0 * ((fConst0 * (2.46091383615691e-17 + (fConst0 * (1.09524566724862e-19 - fConst261)))) - 5.06176860209376e-16)) - 1.00428067549522e-16)));
	fConst263 = (2.96562263790938e-23 * fConst0);
	fConst264 = (1.87941748076643e-17 + (fConst0 * ((fConst0 * ((fConst0 * (2.34900837427787e-17 + (fConst0 * (6.94334841321848e-20 - fConst263)))) - 5.05954368878827e-16)) - 1.00419736024608e-16)));
	fConst265 = (3.75883496153286e-17 + (fConst0 * (2.00536617332493e-16 + (fConst0 * ((fConst0 * (fConst246 - 6.5685493272206e-18)) - 1.0038312416733e-15)))));
	fConst266 = (1.00268308666246e-16 + (fConst0 * (fConst211 - 5.01915620836652e-16)));
	fConst267 = (7.54268684810284e-20 + (fConst0 * ((fConst0 * ((fConst0 * (3.39973252269477e-20 + fConst244)) - 1.00645448513067e-17)) - 2.01172110376171e-18)));
	fConst268 = ((fConst0 * (8.93311991708369e-21 + (fConst0 * (4.47159587641586e-20 - fConst173)))) - 3.34884084364767e-22);
	fConst269 = (fConst0 * (6.74005144979404e-24 + (fConst0 * (1.7976436155422e-22 + (fConst0 * (0 - (8.99340791732735e-22 + fConst244)))))));
	fConst270 = (fConst0 * (6.74005144979404e-24 + (fConst0 * (1.79775482639112e-22 + (fConst0 * (0 - (8.99637353996526e-22 + fConst252)))))));
	fConst271 = ((fConst0 * (8.93419904234241e-21 + (fConst0 * (4.47447354377244e-20 - fConst226)))) - 3.34884084364767e-22);
	fConst272 = (7.54268684810284e-20 + (fConst0 * ((fConst0 * ((fConst0 * (5.05914142301322e-20 + fConst255)) - 1.00678636401925e-17)) - 2.01184555809471e-18)));
	fConst273 = (1.00280390023781e-16 + (fConst0 * ((fConst0 * (fConst257 - 4.8951396009541e-18)) - 5.02237791018395e-16)));
	fConst274 = (4.13471845768615e-17 + (fConst0 * (2.20614513483487e-16 + (fConst0 * ((fConst0 * (fConst259 - 1.04567405339743e-17)) - 1.10486062000205e-15)))));
	fConst275 = (1.87941748076643e-17 + (fConst0 * (1.00428067549522e-16 + (fConst0 * ((fConst0 * ((fConst0 * (1.09524566724862e-19 + fConst261)) - 2.46091383615691e-17)) - 5.06176860209376e-16)))));
	fConst276 = (1.87941748076643e-17 + (fConst0 * (1.00419736024608e-16 + (fConst0 * ((fConst0 * ((fConst0 * (6.94334841321848e-20 + fConst263)) - 2.34900837427787e-17)) - 5.05954368878827e-16)))));
	fConst277 = (1.87941748076643e-17 + (fConst0 * (3.00804925998739e-16 + (fConst0 * (5.01915620836652e-16 + (fConst0 * (0 - (3.2842746636103e-18 + fConst224))))))));
	fConst278 = (1.50402462999369e-16 + (fConst0 * (2.50957810418326e-16 + (fConst0 * (0 - (1.64213733180515e-18 + fConst226))))));
	fConst279 = (1.13140302721543e-19 + (fConst0 * (1.00586055188086e-18 + (fConst0 * ((fConst0 * (0 - (5.09959878404216e-20 + fConst228))) - 5.03227242565336e-18)))));
	fConst280 = ((fConst0 * ((fConst0 * (2.23579793820793e-20 + fConst226)) - 4.46655995854184e-21)) - 5.0232612654715e-22);
	fConst281 = (fConst0 * ((fConst0 * (8.988218077711e-23 + (fConst0 * (1.3490111875991e-21 + fConst228)))) - 3.37002572489702e-24));
	fConst282 = (fConst0 * ((fConst0 * (8.98877413195561e-23 + (fConst0 * (1.34945603099479e-21 + fConst232)))) - 3.37002572489702e-24));
	fConst283 = ((fConst0 * ((fConst0 * (2.23723677188622e-20 + fConst234)) - 4.4670995211712e-21)) - 5.0232612654715e-22);
	fConst284 = (1.13140302721543e-19 + (fConst0 * (1.00592277904735e-18 + (fConst0 * ((fConst0 * (0 - (7.58871213451984e-20 + fConst236))) - 5.03393182009627e-18)))));
	fConst285 = (1.50420585035672e-16 + (fConst0 * (2.51118895509197e-16 + (fConst0 * (0 - (2.44756980047705e-18 + fConst238))))));
	fConst286 = (2.06735922884308e-17 + (fConst0 * (3.3092177022523e-16 + (fConst0 * (5.52430310001027e-16 + (fConst0 * (0 - (5.22837026698717e-18 + fConst240))))))));
	fConst287 = (9.39708740383216e-18 + (fConst0 * (1.50642101324283e-16 + (fConst0 * (2.53088430104688e-16 + (fConst0 * ((fConst0 * (0 - (1.64286850087293e-19 + fConst242))) - 1.23045691807846e-17)))))));
	fConst288 = (9.39708740383216e-18 + (fConst0 * (1.50629604036912e-16 + (fConst0 * (2.52977184439414e-16 + (fConst0 * ((fConst0 * (0 - (1.04150226198277e-19 + fConst244))) - 1.17450418713893e-17)))))));
	fConst289 = (fConst0 * (8.07823996672174e-20 + fConst196));
	fConst290 = (fConst289 - 2.42344177450928e-20);
	fConst291 = (1.11210848921602e-25 * fConst0);
	fConst292 = (fConst0 * (fConst291 - 3.70702829738672e-27));
	fConst293 = (0 - (3.59708419572341e-25 + fConst201));
	fConst294 = (fConst0 * (4.14847776645656e-23 + (fConst0 * (4.14875208655057e-22 + (fConst0 * (fConst244 - 1.93506877123587e-24))))));
	fConst295 = ((fConst0 * (4.27834472507198e-20 + (fConst0 * (1.20130350979737e-20 - fConst224)))) - 1.23860939037882e-20);
	fConst296 = (2.54862445979911e-21 * fConst0);
	fConst297 = ((fConst0 * (1.30237550868038e-18 + (fConst0 * (6.31048104939148e-18 - fConst296)))) - 1.54080357336958e-19);
	fConst298 = (1.03796792326828e-23 * fConst0);
	fConst299 = ((fConst0 * (1.30250876475573e-18 + (fConst0 * (6.31389575293003e-18 + (fConst0 * (fConst298 - 5.25209294198177e-20)))))) - 1.54080357336958e-19);
	fConst300 = (6.04310144881533e-22 * fConst0);
	fConst301 = ((fConst0 * (4.11987720343872e-19 + (fConst0 * (1.61567921603517e-18 - fConst300)))) - 6.30094861372412e-20);
	fConst302 = (7.41405659477344e-26 * fConst0);
	fConst303 = (fConst0 * (0 - (7.41405659477344e-27 + fConst302)));
	fConst304 = (7.19416839144683e-24 * fConst0);
	fConst305 = (7.19416839144683e-25 - fConst304);
	fConst306 = (fConst0 * ((fConst0 * (8.29750417310114e-22 + (fConst0 * (1.29004584749058e-24 - fConst263)))) - 8.29695553291313e-23));
	fConst307 = ((fConst0 * ((fConst0 * (2.40260701959475e-20 + fConst246)) - 8.55668945014395e-20)) - 8.25739593585878e-21);
	fConst308 = (1.69908297319941e-21 * fConst0);
	fConst309 = ((fConst0 * ((fConst0 * (1.2620962098783e-17 + fConst308)) - 2.60475101736075e-18)) - 1.02720238224639e-19);
	fConst310 = (2.07593584653656e-23 * fConst0);
	fConst311 = ((fConst0 * ((fConst0 * (1.26277915058601e-17 + (fConst0 * (3.50139529465451e-20 - fConst310)))) - 2.60501752951147e-18)) - 1.02720238224639e-19);
	fConst312 = (4.02873429921022e-22 * fConst0);
	fConst313 = ((fConst0 * ((fConst0 * (3.23135843207033e-18 + fConst312)) - 8.23975440687743e-19)) - 4.20063240914941e-20);
	fConst314 = (2.01436714960511e-23 * fConst0);
	fConst315 = ((fConst0 * (fConst314 - 1.61564799334435e-19)) - 1.61562784967285e-20);
	fConst316 = (1.61562784967285e-20 + (fConst0 * (0 - (1.61564799334435e-19 + fConst314))));
	fConst317 = (fConst0 * (7.41405659477344e-27 - fConst302));
	fConst318 = (7.19416839144683e-25 + fConst304);
	fConst319 = (fConst0 * ((fConst0 * ((fConst0 * (1.29004584749058e-24 + fConst263)) - 8.29750417310114e-22)) - 8.29695553291313e-23));
	fConst320 = (8.25739593585878e-21 + (fConst0 * ((fConst0 * (fConst246 - 2.40260701959475e-20)) - 8.55668945014395e-20)));
	fConst321 = (1.02720238224639e-19 + (fConst0 * ((fConst0 * (fConst308 - 1.2620962098783e-17)) - 2.60475101736075e-18)));
	fConst322 = (1.02720238224639e-19 + (fConst0 * ((fConst0 * ((fConst0 * (3.50139529465451e-20 + fConst310)) - 1.26277915058601e-17)) - 2.60501752951147e-18)));
	fConst323 = (4.20063240914941e-20 + (fConst0 * ((fConst0 * (fConst312 - 3.23135843207033e-18)) - 8.23975440687743e-19)));
	fConst324 = (2.42344177450928e-20 + fConst197);
	fConst325 = (fConst0 * (3.70702829738672e-27 + fConst291));
	fConst326 = (fConst0 * (4.14847776645656e-23 + (fConst0 * ((fConst0 * (0 - (1.93506877123587e-24 + fConst244))) - 4.14875208655057e-22))));
	fConst327 = (1.23860939037882e-20 + (fConst0 * (4.27834472507198e-20 + (fConst0 * (0 - (1.20130350979737e-20 + fConst224))))));
	fConst328 = (1.54080357336958e-19 + (fConst0 * (1.30237550868038e-18 + (fConst0 * (0 - (6.31048104939148e-18 + fConst296))))));
	fConst329 = (1.54080357336958e-19 + (fConst0 * (1.30250876475573e-18 + (fConst0 * ((fConst0 * (0 - (5.25209294198177e-20 + fConst298))) - 6.31389575293003e-18)))));
	fConst330 = (6.30094861372412e-20 + (fConst0 * (4.11987720343872e-19 + (fConst0 * (0 - (1.61567921603517e-18 + fConst300))))));
	fConst331 = (8.07813924836426e-21 + fConst289);
	fConst332 = (fConst0 * (0 - (3.70702829738672e-27 + fConst199)));
	fConst333 = (fConst0 * (4.14847776645656e-23 + (fConst0 * (4.14875208655057e-22 + (fConst0 * (6.45022923745289e-25 + fConst194))))));
	fConst334 = (4.12869796792939e-21 + (fConst0 * (4.27834472507198e-20 + (fConst0 * (1.20130350979737e-20 + fConst173)))));
	fConst335 = (5.13601191123194e-20 + (fConst0 * (1.30237550868038e-18 + (fConst0 * (6.31048104939148e-18 + fConst205)))));
	fConst336 = (5.13601191123194e-20 + (fConst0 * (1.30250876475573e-18 + (fConst0 * (6.31389575293003e-18 + (fConst0 * (1.75069764732726e-20 + fConst207)))))));
	fConst337 = (2.10031620457471e-20 + (fConst0 * (4.11987720343872e-19 + (fConst0 * (1.61567921603517e-18 + fConst209)))));
	clear_state_f();
}

void Dsp::init_static(uint32_t samplingFreq, PluginLV2 *p)
{
	static_cast<Dsp*>(p)->init(samplingFreq);
}

void always_inline Dsp::compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0)
{
#define fslider0 (*fslider0_)
#define fslider1 (*fslider1_)
#define fslider2 (*fslider2_)
#define fslider3 (*fslider3_)
#define fslider4 (*fslider4_)
	double 	fSlow0 = (0.007000000000000006 * double(fslider0));
	double 	fSlow1 = (0.007000000000000006 * double(fslider1));
	double 	fSlow2 = (0.007000000000000006 * double(fslider2));
	double 	fSlow3 = (0.007000000000000006 * double(fslider3));
	int 	iSlow4 = int(double(fslider4));
	for (int i=0; i<count; i++) {
		fRec0[0] = ((0.993 * fRec0[1]) + fSlow0);
		fRec1[0] = ((0.993 * fRec1[1]) + fSlow1);
		fRec2[0] = ((0.993 * fRec2[1]) + fSlow2);
		double fTemp0 = ((((fConst23 * fRec0[0]) + (fRec1[0] * ((fConst21 * fRec0[0]) + fConst19))) + (fRec2[0] * (8.10491269352103e-19 + ((fRec1[0] * (8.10491269352103e-19 + (fConst0 * (fConst17 + (fConst15 * fRec0[0]))))) + (fConst0 * (((fRec2[0] * (((fRec1[0] * (fConst13 + (fConst11 * fRec0[0]))) + (fConst9 * fRec0[0])) + fConst8)) + (fConst7 * fRec0[0])) + fConst5)))))) + fConst3);
		double fTemp1 = (fConst28 * fRec0[0]);
		double fTemp2 = (fConst31 * fRec0[0]);
		fRec4[0] = ((0.993 * fRec4[1]) + fSlow3);
		double fTemp3 = ((0 - (8.68034563926398e-09 * fRec4[0])) - 8.68034563926398e-11);
		fRec5[0] = ((double)input0[i] - (fConst122 * ((fConst120 * fRec5[1]) + (fConst118 * fRec5[2]))));
		double fTemp4 = (fConst123 * (((fRec5[0] * fTemp3) + (fRec5[1] * (1.7360691278528e-10 + (1.7360691278528e-08 * fRec4[0])))) + (fRec5[2] * fTemp3)));
		fRec3[0] = (fTemp4 - ((((((fRec3[1] * ((((fConst116 * fRec0[0]) + (fRec1[0] * ((fConst115 * fRec0[0]) + fConst114))) + (fRec2[0] * (4.05245634676051e-18 + ((fRec1[0] * (4.05245634676051e-18 + (fConst0 * (fConst113 + (fConst112 * fRec0[0]))))) + (fConst0 * (((fRec2[0] * (((fRec1[0] * (fConst111 + (fConst110 * fRec0[0]))) + (fConst109 * fRec0[0])) + fConst108)) + (fConst107 * fRec0[0])) + fConst106)))))) + fConst105)) + (fRec3[2] * ((((fConst104 * fRec0[0]) + (fRec1[0] * ((fConst103 * fRec0[0]) + fConst102))) + (fRec2[0] * (8.10491269352103e-18 + ((fRec1[0] * (8.10491269352103e-18 + (fConst0 * (fConst101 + (fConst100 * fRec0[0]))))) + (fConst0 * (((fRec2[0] * (((fRec1[0] * (fConst99 + (fConst98 * fRec0[0]))) + (fConst97 * fRec0[0])) + fConst96)) + (fConst95 * fRec0[0])) + fConst94)))))) + fConst93))) + (fRec3[3] * ((((fConst92 * fRec0[0]) + (fRec1[0] * ((fConst90 * fRec0[0]) + fConst88))) + (fRec2[0] * (8.10491269352103e-18 + ((fRec1[0] * (8.10491269352103e-18 + (fConst0 * (fConst86 + (fConst84 * fRec0[0]))))) + (fConst0 * (((fRec2[0] * (((fRec1[0] * (fConst82 + (fConst81 * fRec0[0]))) + (fConst79 * fRec0[0])) + fConst78)) + (fConst77 * fRec0[0])) + fConst76)))))) + fConst75))) + (fRec3[4] * ((((fConst73 * fRec0[0]) + (fRec1[0] * ((fConst71 * fRec0[0]) + fConst69))) + (fRec2[0] * (4.05245634676051e-18 + ((fRec1[0] * (4.05245634676051e-18 + (fConst0 * (fConst67 + (fConst65 * fRec0[0]))))) + (fConst0 * (((fRec2[0] * (((fRec1[0] * (fConst63 + (fConst61 * fRec0[0]))) + (fConst59 * fRec0[0])) + fConst58)) + (fConst57 * fRec0[0])) + fConst55)))))) + fConst53))) + (fRec3[5] * ((((fConst51 * fRec0[0]) + (fRec1[0] * ((fConst50 * fRec0[0]) + fConst49))) + (fRec2[0] * (8.10491269352103e-19 + ((fRec1[0] * (8.10491269352103e-19 + (fConst0 * (fConst48 + (fConst47 * fRec0[0]))))) + (fConst0 * (((fRec2[0] * (((fRec1[0] * (fConst46 + (fConst45 * fRec0[0]))) + (fConst44 * fRec0[0])) + fConst43)) + (fConst42 * fRec0[0])) + fConst41)))))) + fConst40))) / fTemp0));
		double fTemp5 = (fConst127 * fRec0[0]);
		double fTemp6 = (fConst129 * fRec0[0]);
		double fTemp7 = (fConst138 * fRec0[0]);
		double fTemp8 = (fConst141 * fRec0[0]);
		double fTemp9 = (fConst152 * fRec0[0]);
		double fTemp10 = (fConst154 * fRec0[0]);
		double fTemp11 = (fConst160 * fRec0[0]);
		double fTemp12 = (fConst161 * fRec0[0]);
		double fTemp13 = (fConst167 * fRec0[0]);
		double fTemp14 = (fConst168 * fRec0[0]);
		double fTemp15 = ((((fConst195 * fRec0[0]) + (fRec1[0] * ((fConst193 * fRec0[0]) + fConst191))) + (fRec2[0] * (1.87941748076643e-18 + ((fRec1[0] * (1.87941748076643e-18 + (fConst0 * (fConst189 + (fConst187 * fRec0[0]))))) + (fConst0 * (((fRec2[0] * (((fRec1[0] * (fConst185 + (fConst183 * fRec0[0]))) + (fConst181 * fRec0[0])) + fConst180)) + (fConst179 * fRec0[0])) + fConst177)))))) + fConst175);
		double fTemp16 = (fConst200 * fRec0[0]);
		double fTemp17 = (fConst203 * fRec0[0]);
		fRec6[0] = (fTemp4 - ((((((fRec6[1] * ((((fConst288 * fRec0[0]) + (fRec1[0] * ((fConst287 * fRec0[0]) + fConst286))) + (fRec2[0] * (9.39708740383216e-18 + ((fRec1[0] * (9.39708740383216e-18 + (fConst0 * (fConst285 + (fConst284 * fRec0[0]))))) + (fConst0 * (((fRec2[0] * (((fRec1[0] * (fConst283 + (fConst282 * fRec0[0]))) + (fConst281 * fRec0[0])) + fConst280)) + (fConst279 * fRec0[0])) + fConst278)))))) + fConst277)) + (fRec6[2] * ((((fConst276 * fRec0[0]) + (fRec1[0] * ((fConst275 * fRec0[0]) + fConst274))) + (fRec2[0] * (1.87941748076643e-17 + ((fRec1[0] * (1.87941748076643e-17 + (fConst0 * (fConst273 + (fConst272 * fRec0[0]))))) + (fConst0 * (((fRec2[0] * (((fRec1[0] * (fConst271 + (fConst270 * fRec0[0]))) + (fConst269 * fRec0[0])) + fConst268)) + (fConst267 * fRec0[0])) + fConst266)))))) + fConst265))) + (fRec6[3] * ((((fConst264 * fRec0[0]) + (fRec1[0] * ((fConst262 * fRec0[0]) + fConst260))) + (fRec2[0] * (1.87941748076643e-17 + ((fRec1[0] * (1.87941748076643e-17 + (fConst0 * (fConst258 + (fConst256 * fRec0[0]))))) + (fConst0 * (((fRec2[0] * (((fRec1[0] * (fConst254 + (fConst253 * fRec0[0]))) + (fConst251 * fRec0[0])) + fConst250)) + (fConst249 * fRec0[0])) + fConst248)))))) + fConst247))) + (fRec6[4] * ((((fConst245 * fRec0[0]) + (fRec1[0] * ((fConst243 * fRec0[0]) + fConst241))) + (fRec2[0] * (9.39708740383216e-18 + ((fRec1[0] * (9.39708740383216e-18 + (fConst0 * (fConst239 + (fConst237 * fRec0[0]))))) + (fConst0 * (((fRec2[0] * (((fRec1[0] * (fConst235 + (fConst233 * fRec0[0]))) + (fConst231 * fRec0[0])) + fConst230)) + (fConst229 * fRec0[0])) + fConst227)))))) + fConst225))) + (fRec6[5] * ((((fConst223 * fRec0[0]) + (fRec1[0] * ((fConst222 * fRec0[0]) + fConst221))) + (fRec2[0] * (1.87941748076643e-18 + ((fRec1[0] * (1.87941748076643e-18 + (fConst0 * (fConst220 + (fConst219 * fRec0[0]))))) + (fConst0 * (((fRec2[0] * (((fRec1[0] * (fConst218 + (fConst217 * fRec0[0]))) + (fConst216 * fRec0[0])) + fConst215)) + (fConst214 * fRec0[0])) + fConst213)))))) + fConst212))) / fTemp15));
		double fTemp18 = (fConst292 * fRec0[0]);
		double fTemp19 = (fConst294 * fRec0[0]);
		double fTemp20 = (fConst303 * fRec0[0]);
		double fTemp21 = (fConst306 * fRec0[0]);
		double fTemp22 = (fConst317 * fRec0[0]);
		double fTemp23 = (fConst319 * fRec0[0]);
		double fTemp24 = (fConst325 * fRec0[0]);
		double fTemp25 = (fConst326 * fRec0[0]);
		double fTemp26 = (fConst332 * fRec0[0]);
		double fTemp27 = (fConst333 * fRec0[0]);
		output0[i] = (FAUSTFLOAT)((iSlow4)?(fConst0 * (((((((fRec6[0] * ((((fRec1[0] * (fConst337 + (fConst336 * fRec0[0]))) + (fConst335 * fRec0[0])) + (fRec2[0] * (fConst334 + ((fTemp27 + (fRec1[0] * (fConst334 + fTemp27))) + (fConst0 * (fRec2[0] * (fConst293 + (fTemp26 + (fRec1[0] * (fConst293 + fTemp26)))))))))) + fConst331)) + (fRec6[1] * ((((fRec1[0] * (fConst330 + (fConst329 * fRec0[0]))) + (fConst328 * fRec0[0])) + (fRec2[0] * (fConst327 + ((fTemp25 + (fRec1[0] * (fConst327 + fTemp25))) + (fConst0 * (fRec2[0] * (fConst202 + (fTemp24 + (fRec1[0] * (fConst202 + fTemp24)))))))))) + fConst324))) + (fRec6[2] * ((((fRec1[0] * (fConst323 + (fConst322 * fRec0[0]))) + (fConst321 * fRec0[0])) + (fRec2[0] * (fConst320 + ((fTemp23 + (fRec1[0] * (fConst320 + fTemp23))) + (fConst0 * (fRec2[0] * (fConst318 + (fTemp22 + (fRec1[0] * (fConst318 + fTemp22)))))))))) + fConst316))) + (fRec6[3] * (fConst315 + (((fRec1[0] * (fConst313 + (fConst311 * fRec0[0]))) + (fConst309 * fRec0[0])) + (fRec2[0] * (fConst307 + ((fTemp21 + (fRec1[0] * (fConst307 + fTemp21))) + (fConst0 * (fRec2[0] * (fConst305 + (fTemp20 + (fRec1[0] * (fConst305 + fTemp20))))))))))))) + (fRec6[4] * ((((fRec1[0] * (fConst301 + (fConst299 * fRec0[0]))) + (fConst297 * fRec0[0])) + (fRec2[0] * (fConst295 + ((fTemp19 + (fRec1[0] * (fConst295 + fTemp19))) + (fConst0 * (fRec2[0] * (fConst293 + (fTemp18 + (fRec1[0] * (fConst293 + fTemp18)))))))))) + fConst290))) + (fRec6[5] * ((((fRec1[0] * (fConst210 + (fConst208 * fRec0[0]))) + (fConst206 * fRec0[0])) + (fRec2[0] * (fConst204 + ((fTemp17 + (fRec1[0] * (fConst204 + fTemp17))) + (fConst0 * (fRec2[0] * (fConst202 + (fTemp16 + (fRec1[0] * (fConst202 + fTemp16)))))))))) + fConst198))) / fTemp15)):(fConst0 * (((((((fRec3[0] * ((((fRec1[0] * (fConst172 + (fConst171 * fRec0[0]))) + (fConst170 * fRec0[0])) + (fRec2[0] * (fConst169 + ((fTemp14 + (fRec1[0] * (fConst169 + fTemp14))) + (fConst0 * (fRec2[0] * (fConst128 + (fTemp13 + (fRec1[0] * (fConst128 + fTemp13)))))))))) + fConst166)) + (fRec3[1] * ((((fRec1[0] * (fConst165 + (fConst164 * fRec0[0]))) + (fConst163 * fRec0[0])) + (fRec2[0] * (fConst162 + ((fTemp12 + (fRec1[0] * (fConst162 + fTemp12))) + (fConst0 * (fRec2[0] * (fConst30 + (fTemp11 + (fRec1[0] * (fConst30 + fTemp11)))))))))) + fConst159))) + (fRec3[2] * ((((fRec1[0] * (fConst158 + (fConst157 * fRec0[0]))) + (fConst156 * fRec0[0])) + (fRec2[0] * (fConst155 + ((fTemp10 + (fRec1[0] * (fConst155 + fTemp10))) + (fConst0 * (fRec2[0] * (fConst153 + (fTemp9 + (fRec1[0] * (fConst153 + fTemp9)))))))))) + fConst151))) + (fRec3[3] * (fConst150 + (((fRec1[0] * (fConst148 + (fConst146 * fRec0[0]))) + (fConst144 * fRec0[0])) + (fRec2[0] * (fConst142 + ((fTemp8 + (fRec1[0] * (fConst142 + fTemp8))) + (fConst0 * (fRec2[0] * (fConst140 + (fTemp7 + (fRec1[0] * (fConst140 + fTemp7))))))))))))) + (fRec3[4] * ((((fRec1[0] * (fConst136 + (fConst134 * fRec0[0]))) + (fConst132 * fRec0[0])) + (fRec2[0] * (fConst130 + ((fTemp6 + (fRec1[0] * (fConst130 + fTemp6))) + (fConst0 * (fRec2[0] * (fConst128 + (fTemp5 + (fRec1[0] * (fConst128 + fTemp5)))))))))) + fConst125))) + (fRec3[5] * ((((fRec1[0] * (fConst38 + (fConst36 * fRec0[0]))) + (fConst34 * fRec0[0])) + (fRec2[0] * (fConst32 + ((fTemp2 + (fRec1[0] * (fConst32 + fTemp2))) + (fConst0 * (fRec2[0] * (fConst30 + (fTemp1 + (fRec1[0] * (fConst30 + fTemp1)))))))))) + fConst26))) / fTemp0)));
		// post processing
		for (int i=5; i>0; i--) fRec6[i] = fRec6[i-1];
		for (int i=5; i>0; i--) fRec3[i] = fRec3[i-1];
		fRec5[2] = fRec5[1]; fRec5[1] = fRec5[0];
		fRec4[1] = fRec4[0];
		fRec2[1] = fRec2[0];
		fRec1[1] = fRec1[0];
		fRec0[1] = fRec0[0];
	}
#undef fslider0
#undef fslider1
#undef fslider2
#undef fslider3
#undef fslider4
}
		
void __rt_func Dsp::compute_static(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0, PluginLV2 *p)
{
	static_cast<Dsp*>(p)->compute(count, input0, output0);
}


void Dsp::connect(uint32_t port,void* data)
{
	switch ((PortIndex)port)
	{
	case BASS: 
		fslider0_ = (float*)data; // , 0.5, 0.0, 1.0, 0.01 
		break;
	case LEVEL: 
		fslider3_ = (float*)data; // , 0.5, 0.0, 1.0, 0.01 
		break;
	case MID: 
		fslider1_ = (float*)data; // , 0.5, 0.0, 1.0, 0.01 
		break;
	// static const value_pair fslider4_values[] = {{"Hi"},{"Lo"},{0}};
	case TONE: 
		fslider4_ = (float*)data; // , 0.0, 0.0, 1.0, 1.0 
		break;
	case TREBLE: 
		fslider2_ = (float*)data; // , 0.5, 0.0, 1.0, 0.01 
		break;
	default:
		break;
	}
}

void Dsp::connect_static(uint32_t port,void* data, PluginLV2 *p)
{
	static_cast<Dsp*>(p)->connect(port, data);
}


PluginLV2 *plugin() {
	return new Dsp();
}

void Dsp::del_instance(PluginLV2 *p)
{
	delete static_cast<Dsp*>(p);
}

/*
typedef enum
{
   BASS, 
   LEVEL, 
   MID, 
   TONE, 
   TREBLE, 
} PortIndex;
*/

} // end namespace tonemender
