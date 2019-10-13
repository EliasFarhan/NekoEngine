#include <net_prediction_simulation/pred_sim.h>

int main()
{
	net::PredSimEngine engine;
	engine.Init();
	engine.EngineLoop();
	return EXIT_SUCCESS;
}