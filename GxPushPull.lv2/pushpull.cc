// generated from file './/pushpull.dsp' by dsp2cc:
// Code generated with Faust 0.9.73 (http://faust.grame.fr)


namespace pushpull {

class Dsp: public PluginLV2 {
private:
	uint32_t fSamplingFreq;
	class SIG0 {
	  private:
		int 	fSamplingFreq;
		static double 	fWave0[100];
		int 	idxfWave0;
	  public:
		int getNumInputs() 	{ return 0; }
		int getNumOutputs() 	{ return 1; }
		void init(int samplingFreq) {
			fSamplingFreq = samplingFreq;
			idxfWave0 = 0;
		}
		void fill (int count, double output[]) {
			for (int i=0; i<count; i++) {
				output[i] = fWave0[idxfWave0];
				// post processing
				idxfWave0 = (idxfWave0 + 1) % 100;
			}
		}
	};
			double 	fConst0;
	double 	fConst1;
	double 	fConst2;
	double 	fConst3;
	double 	fConst4;
	double 	fConst5;
	double 	fConst6;
	double 	fConst7;
	double 	fConst8;
	double 	fConst9;
	FAUSTFLOAT 	fslider0;
	FAUSTFLOAT	*fslider0_;
	double 	fRec1[2];
	double 	fConst10;
	double 	fConst11;
	double 	fConst12;
	double 	fConst13;
	double 	fConst14;
	double 	fConst15;
	double 	fRec2[3];
	static double 	ftbl0[100];
	double 	fRec0[5];
	double 	fConst16;
	double 	fConst17;
	double 	fConst18;
	double 	fConst19;
	double 	fConst20;
	double 	fConst21;
	double 	fConst22;
	FAUSTFLOAT 	fslider1;
	FAUSTFLOAT	*fslider1_;
	double 	fRec3[2];
	double 	fConst23;
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


double Dsp::ftbl0[100];

Dsp::Dsp()
	: PluginLV2() {
	version = PLUGINLV2_VERSION;
	id = "pushpull";
	name = N_("PushPull");
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
	for (int i=0; i<2; i++) fRec1[i] = 0;
	for (int i=0; i<3; i++) fRec2[i] = 0;
	for (int i=0; i<5; i++) fRec0[i] = 0;
	for (int i=0; i<2; i++) fRec3[i] = 0;
}

void Dsp::clear_state_f_static(PluginLV2 *p)
{
	static_cast<Dsp*>(p)->clear_state_f();
}

inline void Dsp::init(uint32_t samplingFreq)
{
	SIG0 sig0;
	sig0.init(samplingFreq);
	sig0.fill(100,ftbl0);
	fSamplingFreq = samplingFreq;
	fConst0 = double(min(192000, max(1, fSamplingFreq)));
	fConst1 = (1.87266615741707e-19 * fConst0);
	fConst2 = (2.18228451420259e-12 + (fConst0 * ((fConst0 * (5.15102461879573e-15 + (fConst0 * (fConst1 - 5.33404188823928e-17)))) - 1.90894230388643e-13)));
	fConst3 = (7.49066462966829e-19 * fConst0);
	fConst4 = faustpower<2>(fConst0);
	fConst5 = (8.72913805681038e-12 + (fConst0 * ((fConst4 * (1.06680837764786e-16 - fConst3)) - 3.81788460777287e-13)));
	fConst6 = (1.30937070852156e-11 + (fConst4 * ((1.12359969445024e-18 * fConst4) - 1.03020492375915e-14)));
	fConst7 = (8.72913805681038e-12 + (fConst0 * (3.81788460777287e-13 + (fConst4 * (0 - (1.06680837764786e-16 + fConst3))))));
	fConst8 = (2.18228451420259e-12 + (fConst0 * (1.90894230388643e-13 + (fConst0 * (5.15102461879573e-15 + (fConst0 * (5.33404188823928e-17 + fConst1)))))));
	fConst9 = (1.0 / fConst8);
	fConst10 = (1.58757422125848e-09 * fConst0);
	fConst11 = (0 - (7.85562273624188e-09 + fConst10));
	fConst12 = (1.65619207828024e-09 * fConst0);
	fConst13 = (1.09309095675917e-07 + fConst12);
	fConst14 = (7.85562273624188e-09 - fConst10);
	fConst15 = (fConst12 - 1.09309095675917e-07);
	fConst16 = (9.85296585477106e-20 * fConst0);
	fConst17 = (8.9872891852609e-16 + (fConst0 * (fConst16 - 1.90854988538378e-17)));
	fConst18 = (3.94118634190843e-19 * fConst0);
	fConst19 = (fConst0 * (3.81709977076756e-17 - fConst18));
	fConst20 = ((5.91177951286264e-19 * fConst4) - 1.79745783705218e-15);
	fConst21 = (fConst0 * (0 - (3.81709977076756e-17 + fConst18)));
	fConst22 = (8.9872891852609e-16 + (fConst0 * (1.90854988538378e-17 + fConst16)));
	fConst23 = (fConst4 / fConst8);
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
	double 	fSlow0 = (0.007000000000000006 * double(fslider0));
	double 	fSlow1 = (0.007000000000000006 * double(fslider1));
	for (int i=0; i<count; i++) {
		fRec1[0] = ((0.993 * fRec1[1]) + fSlow0);
		double fTemp0 = (5.46545478379584e-06 + (fConst0 * (8.28096039140122e-08 + (fRec1[0] * (fConst13 + (fConst11 * fRec1[0]))))));
		double fTemp1 = (fConst0 * (fRec1[0] * ((0 - (1.64496740098878e-11 * fRec1[0])) - 1.63930088502307e-08)));
		fRec2[0] = ((double)input0[i] - (((fRec2[1] * (1.09309095675917e-05 + (fConst4 * (fRec1[0] * ((3.17514844251696e-09 * fRec1[0]) - 3.31238415656049e-09))))) + (fRec2[2] * (5.46545478379584e-06 + (fConst0 * ((fRec1[0] * (fConst15 + (fConst14 * fRec1[0]))) - 8.28096039140122e-08))))) / fTemp0));
		double fTemp2 = (fConst0 * ((((fRec2[0] * (fTemp1 - 8.19650442511535e-07)) + (fConst0 * ((fRec1[0] * fRec2[1]) * (3.27860177004614e-08 + (3.28993480197756e-11 * fRec1[0]))))) + (fRec2[2] * (8.19650442511535e-07 + fTemp1))) / fTemp0));
		double fTemp3 = fabs(fTemp2);
		double fTemp4 = (101.97 * (fTemp3 / (3.0 + fTemp3)));
		double fTemp5 = max((double)0, min((double)99, floor(fTemp4)));
		double fTemp6 = ((int((0 < fTemp5)))?0:((int((fTemp5 < 99)))?(fTemp4 - fTemp5):99));
		fRec0[0] = ((fabs(((ftbl0[int(fTemp5)] * (1 - fTemp6)) + (fTemp6 * ftbl0[int((1 + fTemp5))]))) * ((int(((fTemp3 * ((int((fTemp2 < 0)))?1:-1)) < 0)))?-1:1)) - (fConst9 * ((((fConst7 * fRec0[1]) + (fConst6 * fRec0[2])) + (fConst5 * fRec0[3])) + (fConst2 * fRec0[4]))));
		fRec3[0] = ((0.993 * fRec3[1]) + fSlow1);
		output0[i] = (FAUSTFLOAT)(fConst23 * (fRec3[0] * (((((fConst22 * fRec0[0]) + (fConst21 * fRec0[1])) + (fConst20 * fRec0[2])) + (fConst19 * fRec0[3])) + (fConst17 * fRec0[4]))));
		// post processing
		fRec3[1] = fRec3[0];
		for (int i=4; i>0; i--) fRec0[i] = fRec0[i-1];
		fRec2[2] = fRec2[1]; fRec2[1] = fRec2[0];
		fRec1[1] = fRec1[0];
	}
#undef fslider0
#undef fslider1
}
	
	
	double 	Dsp::SIG0::fWave0[100] = {
	0.0,0.0252525303042,0.0505050554952,0.0757575805852,0.101010105408,
	0.126262629527,0.151515151785,0.176767669127,0.202020173487,0.227272643558,
	0.252525023071,0.27777716344,0.30302867248,0.328278516174,0.353523976201,
	0.378757960762,0.403962331699,0.429092973801,0.454054762955,0.478689455764,
	0.502836733588,0.526445276195,0.549584906934,0.572366805873,0.594888006261,
	0.617219241416,0.639409501226,0.661492650438,0.683492486112,0.705426107436,
	0.727306087679,0.749141876788,0.770940721036,0.792708278216,0.814449037946,
	0.836166614854,0.857863957123,0.879543497557,0.90120726487,0.922856966935,
	0.944494053967,0.966119767075,0.98773517605,1.00934120908,1.03093867636,
	1.05252828904,1.07411067455,1.09568638907,1.11725592785,1.13881973368,
	1.16037820399,1.18193169674,1.20348053549,1.22502501359,1.24656539782,
	1.26810193145,1.28963483693,1.31116431816,1.3326905625,1.35421374248,
	1.37573401732,1.39725153427,1.41876642975,1.44027883039,1.46178885393,
	1.48329661007,1.5048022011,1.52630572263,1.5478072641,1.56930690934,
	1.59080473699,1.61230082094,1.63379523069,1.65528803169,1.67677928567,
	1.69826905088,1.71975738237,1.74124433221,1.7627299497,1.78421428156,
	1.80569737213,1.82717926349,1.84865999562,1.87013960657,1.89161813255,
	1.91309560803,1.9345720659,1.95604753753,1.97752205287,1.99899564052,
	2.02046832719,2.04193999607,2.06337932704,2.08170426416,2.08860470934,
	2.09152655523,2.09332986318,2.09463709766,2.0956676926,2.09652273038
	};

void __rt_func Dsp::compute_static(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0, PluginLV2 *p)
{
	static_cast<Dsp*>(p)->compute(count, input0, output0);
}


void Dsp::connect(uint32_t port,void* data)
{
	switch ((PortIndex)port)
	{
	case DRIVE: 
		fslider0_ = (float*)data; // , 0.5, 0.0, 1.0, 0.01 
		break;
	case VOLUME: 
		fslider1_ = (float*)data; // , 0.5, 0.0, 1.0, 0.01 
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
   DRIVE, 
   VOLUME, 
} PortIndex;
*/

} // end namespace pushpull
