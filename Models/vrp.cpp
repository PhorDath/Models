#include "vrp.h"

void vrp::varX(GRBModel &model)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			x.at(i).at(j) = model.addVar(0, 1, 1, GRB_BINARY, "x(" + to_string(i) + "," + to_string(j) + ")");
		}
	}
	model.update();
}

void vrp::varF(GRBModel &model)
{
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			f.at(i).at(j) = model.addVar(0, GRB_INFINITY, 1, GRB_INTEGER, "f(" + to_string(i) + "," + to_string(j) + ")");
		}
	}
	model.update();
}

void vrp::fo(GRBModel &model)
{
	GRBLinExpr FO = 0;
	for (int i = 0; i < numClients + 1; i++) {
		for (int j = 0; j < numClients + 1; j++) {
			FO += distance.at(i).at(j) * x.at(i).at(j);
		}
	}
	model.setObjective(FO, GRB_MINIMIZE);
	model.update();
}

void vrp::c1(GRBModel &model)
{
	for (int j = 1; j < numClients + 1; j++) {
		GRBLinExpr c1 = 0;
		// sum of arcs entering each node, excluding the depot
		for (int i = 0; i < numClients + 1; i++) {		
			c1 += x.at(i).at(j);
		}
		model.addConstr(c1 == 1, "c1(" + to_string(j) + ")");
	}
	model.update();
}

void vrp::c2(GRBModel &model)
{
	for (int i = 1; i < numClients + 1; i++) {
		GRBLinExpr c2 = 0;
		// sum of acrs leaving each node, excluding the depot
		for (int j = 0; j < numClients + 1; j++) {
			c2 += x.at(i).at(j);
		}
		model.addConstr(c2 == 1, "c2(" + to_string(i) + ")");
	}
	model.update();
}

void vrp::c3(GRBModel &model)
{
	GRBLinExpr c31 = 0;
	GRBLinExpr c32 = 0;
	// sum of arcs leaving and entering the depot must be the same amount
	for (int k = 0; k < numClients + 1; k++) {
		c31 += x.at(0).at(k);
		c32 += x.at(k).at(0);
	}
	model.addConstr(c31 == c32, "c3");
	model.update();
}

void vrp::c4(GRBModel &model)
{
	for (int k = 1; k < numClients + 1; k++) {
		GRBLinExpr c41 = 0, c42 = 0;
		for (int l = 0; l < numClients + 1; l++) {
			c41 += f.at(l).at(k);
			c42 += f.at(k).at(l);
		}
		model.addConstr(c41 - c42 == q.at(k), "c4(" + to_string(k) + ")");
	}
	model.update();
}

void vrp::c5(GRBModel &model)
{
	for (int i = 0; i < numClients + 1; i++) {
		for (int j = 0; j < numClients + 1; j++) {
			GRBLinExpr c5 = 0;
			model.addConstr(f.at(i).at(j) <= vehicleCapacity * x.at(i).at(j), "c5(" + to_string(i) + "," + to_string(j) + ")");
		}
	}
	model.update();
}

vrp::vrp(string)
{
}

void vrp::setupModel()
{
	GRBEnv env = GRBEnv(true);
	env.start();

	try {
		//env = new GRBEnv();
		GRBModel model = GRBModel(env);
		model.set(GRB_StringAttr_ModelName, "pmedians");

		varX(model);
		fo(model);

		c1(model);
		c2(model);
		c3(model);
		c4(model);

		model.write("teste.lp");

		model.getEnv().set(GRB_DoubleParam_TimeLimit, 600);

		model.optimize();

		getSolution(model);
	}
	catch (GRBException e) {
		cout << "Error code = " << e.getErrorCode() << endl;
		cout << e.getMessage() << endl;
	}
}

vrp::~vrp()
{
}
