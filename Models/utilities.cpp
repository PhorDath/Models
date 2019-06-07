#include "utilities.h"

float utilities::distance(coord a, coord b) {
	return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) * 1.0);
}

void utilities::processSolution(GRBModel &model)
{
	int status = model.get(GRB_IntAttr_Status);

	if (status == GRB_UNBOUNDED)
	{
		cout << "O modelo nao pode ser resolvido porque e ilimitado" << endl;
	}
	if (status == GRB_OPTIMAL)
	{
		cout << "Solucao otima encontrada!" << endl;
		//Acessa e imprime o valor da funcao objetivo

		cout << "O valor da solucao otima e: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	}
	if (status == GRB_TIME_LIMIT)
	{
		cout << "Tempo limite!" << endl;
		//Acessa e imprime o valor da funcao objetivo

		cout << "O valor da melhot solucao ate o momento e: " << model.get(GRB_DoubleAttr_ObjVal) << endl;

	}
	if (status == GRB_INFEASIBLE)
	{
		cout << "O modelo nao pode ser resolvido porque e inviavel" << endl;

	}
	else {
		cout << "Status: " << status << endl;

	}
}
