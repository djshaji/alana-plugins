// generated from file './/foxxtonemachine_p3.dsp' by dsp2cc:
// Code generated with Faust 0.9.73 (http://faust.grame.fr)


namespace foxxtonemachine {

template <int tab_size>
struct table1d_imp { // 1-dimensional clipping table
    float low;
    float high;
    float istep;
    int size;
    float data[tab_size];
};

static table1d_imp<100> clip __rt_data = {
	0.005,0.795235013262,101.97,100, {
	0.00548195523036,0.016445865691,0.0274097761516,0.0383736866119,0.0493375970718,
	0.060301507531,0.0712654179894,0.0822293284464,0.0931932389016,0.104157149354,
	0.115121059803,0.126084970245,0.13704888068,0.148012791103,0.158976701508,
	0.169940611889,0.180904522235,0.19186843253,0.202832342751,0.213796252866,
	0.224760162828,0.23572407257,0.246687981995,0.257651890962,0.26861579927,
	0.279579706628,0.290543612617,0.301507516632,0.312471417804,0.323435314878,
	0.334399206045,0.3453630887,0.356326959087,0.367290811794,0.378254639021,
	0.389218429528,0.400182167115,0.411145828437,0.422109379849,0.433072772871,
	0.444035937638,0.454998773485,0.465961135365,0.476922814321,0.48788350936,
	0.498842787024,0.509800023297,0.520754320256,0.531704386732,0.542648367922,
	0.55358360312,0.564506283341,0.575410971703,0.586289940184,0.597132269761,
	0.607922663898,0.618639951041,0.629255322196,0.639730493837,0.650016225239,
	0.660051925492,0.669767316942,0.679086989359,0.687937882416,0.696258343947,
	0.704006183628,0.711163121782,0.717734492237,0.723745022743,0.72923269117,
	0.734242606851,0.738822090117,0.743017303269,0.74687127245,0.750422944114,
	0.7537069184,0.756753578753,0.759589425229,0.762237491115,0.764717773458,
	0.767047640962,0.769242202516,0.771314630963,0.773276442704,0.775137736666,
	0.776907397287,0.778593266396,0.780202288563,0.781740634041,0.783213802858,
	0.784626713121,0.785983776084,0.787288960129,0.788545845459,0.789757670968,
	0.790927374556,0.792057627879,0.793150866414,0.794209315546,0.795235013262
	}
};

class Dsp: public PluginLV2 {
private:
	uint32_t fSamplingFreq;
	double 	fConst0;
	double 	fConst1;
	double 	fConst2;
	FAUSTFLOAT 	fslider0;
	FAUSTFLOAT	*fslider0_;
	double 	fRec0[2];
	double 	fConst3;
	double 	fConst4;
	double 	fConst5;
	double 	fConst6;
	double 	fConst7;
	double 	fConst8;
	double 	fConst9;
	double 	fConst10;
	double 	fConst11;
	double 	fConst12;
	double 	fConst13;
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
	double 	fRec1[5];
	double 	fConst27;
	double 	fConst28;
	double 	fConst29;
	double 	fConst30;
	double 	fConst31;
	double 	fConst32;
	double 	fConst33;
	double 	fConst34;
	double 	fConst35;
	FAUSTFLOAT 	fslider1;
	FAUSTFLOAT	*fslider1_;
	double 	fRec2[2];
	void connect(uint32_t port,void* data);
	void clear_state_f();
	void init(uint32_t samplingFreq);
	void compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0);

	static void clear_state_f_static(PluginLV2*);
	static void init_static(uint32_t samplingFreq, PluginLV2*);
	static double symclip(double x);
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
	id = "foxtonemachine_p2";
	name = N_("foxtonemachine");
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
	for (int i=0; i<5; i++) fRec1[i] = 0;
	for (int i=0; i<2; i++) fRec2[i] = 0;
}

void Dsp::clear_state_f_static(PluginLV2 *p)
{
	static_cast<Dsp*>(p)->clear_state_f();
}

inline void Dsp::init(uint32_t samplingFreq)
{
	fSamplingFreq = samplingFreq;
	fConst0 = double(min(192000, max(1, fSamplingFreq)));
	fConst1 = (1.43477264906976e-19 * fConst0);
	fConst2 = (9.36724657293001e-13 + (fConst0 * (2.88688849427588e-13 + (fConst0 * (5.26887000642421e-16 + fConst1)))));
	fConst3 = (1.43995096983312e-19 * fConst0);
	fConst4 = (fConst0 * ((fConst0 * (0 - (6.99937482488722e-17 + fConst3))) - 1.75833647137856e-16));
	fConst5 = (1.30459557866881e-19 * fConst0);
	fConst6 = (5.33005067398218e-13 + (fConst0 * (2.1225419964864e-13 + (fConst0 * (4.69269855070471e-16 + fConst5)))));
	fConst7 = (5.80207383051885e-19 * fConst0);
	fConst8 = (fConst0 * (0 - (3.11908059529475e-30 + fConst7)));
	fConst9 = (5.45394940068771e-20 * fConst0);
	fConst10 = ((fConst0 * (4.30385940813149e-16 - fConst9)) - 8.50476014724067e-13);
	fConst11 = ((fConst0 * (2.88688849427588e-13 + (fConst0 * (fConst1 - 5.26887000642421e-16)))) - 9.36724657293001e-13);
	fConst12 = (fConst0 * ((fConst0 * (6.99937482488722e-17 - fConst3)) - 1.75833647137856e-16));
	fConst13 = ((fConst0 * (2.1225419964864e-13 + (fConst0 * (fConst5 - 4.69269855070471e-16)))) - 5.33005067398218e-13);
	fConst14 = (5.73909059627905e-19 * fConst0);
	fConst15 = faustpower<2>(fConst0);
	fConst16 = ((fConst15 * (1.05377400128484e-15 - fConst14)) - 1.873449314586e-12);
	fConst17 = (5.75980387933248e-19 * fConst0);
	fConst18 = (fConst15 * (fConst17 - 1.39987496497744e-16));
	fConst19 = (5.21838231467523e-19 * fConst0);
	fConst20 = ((fConst15 * (9.38539710140941e-16 - fConst19)) - 1.06601013479644e-12);
	fConst21 = (3.51667294275713e-16 - (8.63970581899873e-19 * fConst15));
	fConst22 = ((7.82757347201285e-19 * fConst15) - 4.2450839929728e-13);
	fConst23 = ((8.60863589441858e-19 * fConst15) - 5.77377698855177e-13);
	fConst24 = (1.873449314586e-12 + (fConst15 * (0 - (1.05377400128484e-15 + fConst14))));
	fConst25 = (fConst15 * (1.39987496497744e-16 + fConst17));
	fConst26 = (1.06601013479644e-12 + (fConst15 * (0 - (9.38539710140941e-16 + fConst19))));
	fConst27 = (2.32082953220754e-18 * fConst0);
	fConst28 = (6.23816119058949e-30 + fConst27);
	fConst29 = (2.18157976027508e-19 * fConst0);
	fConst30 = (fConst29 - 8.60771881626297e-16);
	fConst31 = (1.70095202944813e-12 - (3.27236964041263e-19 * fConst15));
	fConst32 = (fConst27 - 6.23816119058949e-30);
	fConst33 = (8.60771881626297e-16 + fConst29);
	fConst34 = (fConst0 * (3.11908059529475e-30 - fConst7));
	fConst35 = ((fConst0 * (0 - (4.30385940813149e-16 + fConst9))) - 8.50476014724067e-13);
	clear_state_f();
}

void Dsp::init_static(uint32_t samplingFreq, PluginLV2 *p)
{
	static_cast<Dsp*>(p)->init(samplingFreq);
}

double always_inline Dsp::symclip(double x) {
    double f = fabs(x);
    f = (f - clip.low) * clip.istep;
    int i = static_cast<int>(f);
    if (i < 0) {
        f = clip.data[0];
    } else if (i >= clip.size-1) {
        f = clip.data[clip.size-1];
    } else {
	f -= i;
	f = clip.data[i]*(1-f) + clip.data[i+1]*f;
    }
    return copysign(f, x);
}

void always_inline Dsp::compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0)
{
#define fslider0 (*fslider0_)
#define fslider1 (*fslider1_)
	double 	fSlow0 = (0.007000000000000006 * double(fslider0));
	double 	fSlow1 = (0.007000000000000006 * double(fslider1));
	for (int i=0; i<count; i++) {
		fRec0[0] = ((0.993 * fRec0[1]) + fSlow0);
		double fTemp0 = (5.33005067398218e-13 + (fConst0 * ((fRec0[0] * (fConst6 + (fConst4 * fRec0[0]))) + fConst2)));
		fRec1[0] = (symclip((double)input0[i]) - (((((fRec1[1] * (2.13202026959287e-12 + (fConst0 * ((fRec0[0] * (fConst26 + (fConst25 * fRec0[0]))) + fConst24)))) + (fRec1[2] * (3.19803040438931e-12 + (fConst15 * (fConst23 + (fRec0[0] * (fConst22 + (fConst21 * fRec0[0])))))))) + (fRec1[3] * (2.13202026959287e-12 + (fConst0 * ((fRec0[0] * (fConst20 + (fConst18 * fRec0[0]))) + fConst16))))) + (fRec1[4] * (5.33005067398218e-13 + (fConst0 * ((fRec0[0] * (fConst13 + (fConst12 * fRec0[0]))) + fConst11))))) / fTemp0));
		fRec2[0] = ((0.993 * fRec2[1]) + fSlow1);
		output0[i] = (FAUSTFLOAT)(fConst15 * ((fRec2[0] * (((((fRec1[0] * (fConst35 + (fConst34 * fRec0[0]))) + (fConst0 * (fRec1[1] * (fConst33 + (fConst32 * fRec0[0]))))) + (fRec1[2] * (fConst31 + (fConst15 * (0 - (3.48124429831131e-18 * fRec0[0])))))) + (fConst0 * (fRec1[3] * (fConst30 + (fConst28 * fRec0[0]))))) + (fRec1[4] * (fConst10 + (fConst8 * fRec0[0]))))) / fTemp0));
		// post processing
		fRec2[1] = fRec2[0];
		for (int i=4; i>0; i--) fRec1[i] = fRec1[i-1];
		fRec0[1] = fRec0[0];
	}
#undef fslider0
#undef fslider1
}

void __rt_func Dsp::compute_static(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0, PluginLV2 *p)
{
	static_cast<Dsp*>(p)->compute(count, input0, output0);
}


void Dsp::connect(uint32_t port,void* data)
{
	switch ((PortIndex)port)
	{
	case TONE: 
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
   TONE, 
   VOLUME, 
} PortIndex;
*/

} // end namespace foxxtonemachine_p3
