#include "seam_carving.h"
#include <string>
#include <QtWidgets/QApplication>

bool demo = 0;
int edge_type = 0;

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	seam_carving w;
	w.show();
	return a.exec();
}
