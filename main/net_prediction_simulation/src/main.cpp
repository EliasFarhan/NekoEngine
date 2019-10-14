#include <net_prediction_simulation/pred_sim_engine.h>

int main()
{
	neko::net::PredSimEngine engine;
	engine.Init();
	engine.EngineLoop();
	return EXIT_SUCCESS;
}