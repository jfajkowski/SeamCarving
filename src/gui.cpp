#include "gui.h"
#include <qmessagebox.h>
#include <qfiledialog.h>

gui::gui(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	this->move(QPoint(0, 0));
	ui.numberV->setMaximum(0);
	ui.numberH->setMaximum(0);
	number_of_vertical_seams = 0;
	number_of_horizontal_seams = 0;
	connect(ui.load, SIGNAL(clicked()), this, SLOT(clickedLoad()));
	connect(ui.add, SIGNAL(clicked()), this, SLOT(clickedAdd()));
	connect(ui.del, SIGNAL(clicked()), this, SLOT(clickedDel()));
	connect(ui.energy, SIGNAL(clicked()), this, SLOT(clickedEnergy()));
	connect(ui.demo, SIGNAL(clicked()), this, SLOT(clickedDemo()));
	connect(ui.numberV, SIGNAL(valueChanged(int)), this, SLOT(changedNumberOfVerticalSeams(int)));
	connect(ui.numberH, SIGNAL(valueChanged(int)), this, SLOT(changedNumberOfHorizontalSeams(int)));
	connect(ui.sobel, SIGNAL(clicked()), this, SLOT(setSobel()));
	connect(ui.scharr, SIGNAL(clicked()), this, SLOT(setScharr()));
	connect(ui.canny, SIGNAL(clicked()), this, SLOT(setCanny()));
}

gui::~gui()
{

}

void gui::errorDialog(std::string s)
{
	QMessageBox messageBox;
	messageBox.critical(0, "Error", QString::fromStdString(s));
	messageBox.setFixedSize(500, 200);
}

void gui::setMaxNumbersOfSeams()
{
	if (i.returnHeight() > 200)
		ui.numberH->setMaximum(i.returnHeight() - 100);
	else
		ui.numberH->setMaximum(0);

	if (i.returnWidth() > 200)
		ui.numberV->setMaximum(i.returnWidth() - 100);
	else
		ui.numberV->setMaximum(0);
}

void gui::clickedLoad()
{
	QString path = QFileDialog::getOpenFileName(this, tr("Open File"), QString(""), QString("All files (*.*);;Picture (*.jpg)"));
	if (path.isEmpty()) return;

	src = imread(path.toStdString());

	if (!src.data)
	{
		errorDialog("Can't open file.");
		return;
	}

	QFileInfo fileInfo(path);
	QString file_name(fileInfo.fileName());

	i = Image(src, file_name.toStdString());

	setMaxNumbersOfSeams();

	namedWindow("Preview", CV_WINDOW_AUTOSIZE);
	i.showImage("Preview");
	cvWaitKey();

}

void gui::clickedAdd()
{
	if (!src.data)
	{
		errorDialog("No picture loaded.");
		return;
	}
	if (number_of_vertical_seams > 0)
	{
		i.findSeams(number_of_vertical_seams);
		i.addSeamsUsingList(demo);
	}
	
	if (number_of_horizontal_seams > 0)
	{
		i.rotateImage();
		i.findSeams(number_of_horizontal_seams);
		i.addSeamsUsingList(demo);
		i.rotateImage();
		i.showImage("Preview");
	}

	setMaxNumbersOfSeams();
}

void gui::clickedDel()
{
	if (!src.data)
	{
		errorDialog("No picture loaded.");
		return;
	}
	i.deleteVerticalSeams(number_of_vertical_seams, "Preview");
	i.showImage("Preview");
	
	i.deleteHorizontalSeams(number_of_horizontal_seams, "Preview");
	i.showImage("Preview");

	setMaxNumbersOfSeams();
}

void gui::clickedEnergy()
{
	if (!src.data)
	{
		errorDialog("No picture loaded.");
		return;
	}

	showEnergyDemo(src);
}

void gui::clickedDemo()
{
	demo = !demo;
}

void gui::changedNumberOfVerticalSeams(int k)
{
	number_of_vertical_seams = k;
	ui.labelV->setText(QString::number(k));
}

void gui::changedNumberOfHorizontalSeams(int k)
{
	number_of_horizontal_seams = k;
	ui.labelH->setText(QString::number(k));
}
