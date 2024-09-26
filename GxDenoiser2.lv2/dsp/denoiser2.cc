// generated from file './/denoiser2.dsp' by dsp2cc:
// Code generated with Faust 0.9.90 (http://faust.grame.fr)

#define FILTER_SIZE (8)

namespace denoiser2 {

class Dsp: public PluginLV2 {
private:
	uint32_t fSamplingFreq;
	double filter_buffer[FILTER_SIZE];
	double filter_sum;
	double *filter_position;
	double filter_buffer2[FILTER_SIZE];
	double filter_sum2;
	double *filter_position2;

	void filter_init();
	double filter(double new_value);
	void filter_init2();
	double filter2(double new_value);
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
	id = "denoise";
	name = N_("Denoise");
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
	
}

void Dsp::clear_state_f_static(PluginLV2 *p)
{
	static_cast<Dsp*>(p)->clear_state_f();
}

inline void Dsp::filter_init() {
	register int i;
	filter_sum = 0;
	filter_position = filter_buffer;
	for(i=0; i<FILTER_SIZE; i++) filter_buffer[i] = 0;
}

inline void Dsp::filter_init2() {
	register int i;
	filter_sum2 = 0;
	filter_position2 = filter_buffer2;
	for(i=0; i<FILTER_SIZE; i++) filter_buffer2[i] = 0;
}

inline void Dsp::init(uint32_t samplingFreq)
{
	fSamplingFreq = samplingFreq;
	filter_init();
	filter_init2();
}

void Dsp::init_static(uint32_t samplingFreq, PluginLV2 *p)
{
	static_cast<Dsp*>(p)->init(samplingFreq);
}

double always_inline Dsp::filter(double new_value) {
	filter_sum -= *filter_position;
	*filter_position = new_value;
	filter_sum += new_value;
	if(++filter_position >= filter_buffer + FILTER_SIZE) {
		filter_position = filter_buffer;
	}
	return filter_sum / FILTER_SIZE;
}

double always_inline Dsp::filter2(double new_value) {
	filter_sum2 -= *filter_position2;
	*filter_position2 = new_value;
	filter_sum2 += new_value;
	if(++filter_position2 >= filter_buffer2 + FILTER_SIZE) {
		filter_position2 = filter_buffer2;
	}
	return filter_sum2 / FILTER_SIZE;
}

void always_inline Dsp::compute(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0)
{
	for (int i=0; i<count; i++) {
		output0[i] = (FAUSTFLOAT)filter(filter2((double)input0[i]));
	}
}

void __rt_func Dsp::compute_static(int count, FAUSTFLOAT *input0, FAUSTFLOAT *output0, PluginLV2 *p)
{
	static_cast<Dsp*>(p)->compute(count, input0, output0);
}


void Dsp::connect(uint32_t port,void* data)
{
	switch ((PortIndex)port)
	{
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
} PortIndex;
*/

} // end namespace denoiser2
