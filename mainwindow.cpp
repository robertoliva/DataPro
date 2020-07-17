#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<QInputDialog>
using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("DataPro (DP)");
    Logo =":/Images/logo/logo4.ico";
    setWindowIcon(QIcon(Logo));

    ui->plot->addGraph();
    ui->plot->graph(0)->setLineStyle(QCPGraph::lsLine);

    QFont pfont("Newyork",6);
    pfont.setStyleHint(QFont::Times);
    pfont.setPointSize(14);
    ui->plot->xAxis->setTickLabelFont(pfont);
    ui->plot->yAxis->setTickLabelFont(pfont);
    ui->plot->xAxis->setLabelFont(pfont);
    ui->plot->yAxis->setLabelFont(pfont);

    x_axis = "x Axis";
    y_axis = "y Axis";
    ui->plot->xAxis->setLabel(x_axis);
    ui->plot->yAxis->setLabel(y_axis);
    //Mouse Drag Zoom-In functionality here
    ui->plot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    ui->plot->axisRect()->setRangeDrag(Qt::Vertical|Qt::Horizontal);
    ui->plot->axisRect()->setRangeZoomAxes(ui->plot->xAxis,ui->plot->yAxis); //To block y axis zoom NULL axis as horizontal
    ui->plot->setSelectionRectMode(QCP::srmZoom);
    ui->plot->setSelectionRectMode(QCP::srmNone);

    ui->plot->legend->setFont(pfont);
    ui->plot->legend->setSelectedFont(pfont);
    ui->plot->clearGraphs();

    // I unsuccesfully tried to move the cursor cross from up, down right left keys: QObject::connect: No such signal QCustomPlot::keyPressEvent
    // ui->plot->setFocusPolicy(Qt::StrongFocus);
    // ui->plot->setFocus();
    //connect(ui->plot, SIGNAL(keyPressEvent(MainWindow::QKeyEvent*event2)), ui->plot, SLOT(keyPressed(MainWindow::QKeyEvent*event2)));

    connect(ui->plot, SIGNAL(mousePress(QMouseEvent*)), SLOT(clickedGraph(QMouseEvent*)));
    connect(ui->plot, SIGNAL(mouseDoubleClick(QMouseEvent*)), SLOT(doubleclickedGraph(QMouseEvent*)));

 //I think this would allow to drag plot instead of zooming with mouse.
    //ui->plot->setInteraction(QCP::iRangeDrag, true);
}

MainWindow::~MainWindow(){
    delete ui;
}

// ************************************************************************************************************
// *********************************************** SUBFUNCTIONS ***********************************************
// ************************************************************************************************************

void MainWindow::getData(QStringList fitxer, QVector<QVector<QVector<QVector<double>>>> &InputData){
    QString WorkingFitxer;
    int error =0;
    InputData.clear();
    InputData.resize(undo); //Because it must be able to undo up to 5 times.
    if(plotNumLim==0){
        plotNum=fitxer.size();
    }else if(plotNumLim<=fitxer.size()){                         // If plotNum is zero plot all files.
        plotNum=plotNumLim;
    }else{
        plotNum=fitxer.size();
    }
    InputData[0].resize(plotNum); //Setting proper size for AllData[0] 3D vector.

    for (int i = 0; i< plotNum; i++){                                                               // Run over all files
        WorkingFitxer = fitxer.at(i);
        QByteArray ba = WorkingFitxer.toLocal8Bit(); //convert fitxer into char c_str2
        const char *c_str2 = ba.data();
        ifstream obre(c_str2, ios::in);
        int m = 0;
        int p = 0;
        InputData[0][i].resize(ColNums(WorkingFitxer));
            for(int k=0;k<ColNums(WorkingFitxer);k++){                                                   // Run over all columns
                InputData[0][i][k].resize(RowNums(WorkingFitxer)+1); //+1 because RowNums counts the first line as 0.
            }

            //QMessageBox::information(this,"Information", QString("Number of cols is %1 and rows is %2") .arg(ColNums(WorkingFitxer)) .arg(RowNums(WorkingFitxer)+1) );

        std::string line;
        while (std::getline(obre, line)) {                  //Run over rows
      //  while (std::safeGetline(obre, line).eof()) {                  //Run over rows

                if(validate_row(line)){                         // It will ignore empty lines or lines with errors.
                    std::istringstream row(line);
                    std::string buffers;
                        while (row>>buffers){                           // Run over columns
                          InputData[0][i][p][m]=atof(buffers.c_str());
                         p++;
                        }
                    p =0;
                    m++;
                }else{
                    error=1;
                }
            }
        obre.close();
        //QMessageBox::information(this,"Information", QString("Number of files is %1 and rows is %2") .arg(i) .arg(m) );
    }
    if(error==1){
      QMessageBox::warning(this,"Warning!", QString("Empty lines or lines with invalid data have been detected. Please make sure that the datafiles content exhibit proper format and numbers.") );
    }
}

int MainWindow::getOneData(QString OneFitxer, QVector<QVector<QVector<QVector<double>>>> &InputData){ //IDEALLY This function should be replaced by GetData function, but it was easier to make this one than adapt the other for plotting and saving purposes.
        int error =0;
        QByteArray ba = OneFitxer.toLocal8Bit(); //convert fitxer into char c_str2
        const char *c_str2 = ba.data();
        ifstream obre(c_str2, ios::in);
        int m = 0;
        int p = 0;
        //QMessageBox::information(this,"Info", QString("OneFitxer size: %1 time %2") .arg(InputData.size()) .arg(1));
        InputData[0].resize(1); //Setting proper size for AllData[0] 3D vector.
        //QMessageBox::information(this,"Info", QString("Works: %1") .arg(1));
            InputData[0][0].resize(ColNums(OneFitxer));
        //QMessageBox::information(this,"Info", QString("ColNums: %1") .arg(ColNums(OneFitxer)));
            for(int k=0;k<ColNums(OneFitxer);k++){
                InputData[0][0][k].resize(RowNums(OneFitxer)+1);
            }
        std::string line;
            while (std::getline(obre, line)) {  //Run over lines
                if(validate_row(line)){         //Will ignore all lines with errors
                    std::istringstream row(line);
                    std::string buffers;
                    while (row>>buffers){                           // Run over columns
                        InputData[0][0][p][m]=atof(buffers.c_str());
                       p++;
                      }
                    p =0;
                    m++;        // Here I should check if the line is valid. For a line to be valid, all its data must be doubles.
                }else{
                    error=1;
                }
            }
        obre.close();
        if(error==1){
          //QMessageBox::information(this,"Information", QString("Warning: Empty lines or lines with invalid data have been detected. Please make sure that the datafiles content exhibit proper format and numbers.") );
          return 1;
        }else{
          return 0;
        }
}

int MainWindow::getOneSingleData(QString fitxer, QVector<QVector<double>> &InputData){
    //i is an optional parameter to get a 4D array with the data in the file position i, i.e. [][i][][].
    int error =0;
    InputData.clear();
        QByteArray ba = fitxer.toLocal8Bit(); //convert fitxer into char c_str2
        const char *c_str2 = ba.data();
        ifstream obre(c_str2, ios::in);
        int m = 0;
        int p = 0;
        InputData.resize(ColNums(fitxer));
            for(int k=0;k<ColNums(fitxer);k++){                                                   // Run over all columns
                InputData[k].resize(RowNums(fitxer)+1); //+1 because RowNums counts the first line as 0.
            }

            //QMessageBox::information(this,"Information", QString("Number of cols is %1 and rows is %2") .arg(ColNums(WorkingFitxer)) .arg(RowNums(WorkingFitxer)+1) );

        std::string line;
            while (std::getline(obre, line)) {                  //Run over rows
                if(validate_row(line)){                         // It will ignore empty lines or lines with errors.
                    std::istringstream row(line);
                    std::string buffers;
                        while (row>>buffers){                           // Run over columns
                          InputData[p][m]=atof(buffers.c_str());
                         p++;
                        }
                    p =0;
                    m++;
                }else{
                    error=1;
                }
            }
        obre.close();
        //QMessageBox::information(this,"Information", QString("Number of files is %1 and rows is %2") .arg(i) .arg(m) );

    if(error==1){
        // QMessageBox::information(this,"Information", QString("Warning: Empty lines or lines with invalid data have been detected. Please make sure that the datafiles content exhibit proper format and numbers.") );
        return 1;
    }else{
        return 0;
    }
}

int MainWindow::getSingleData(QString fitxer, int i, QVector<QVector<QVector<QVector<double>>>> &InputData){
    //i is an optional parameter to get a 4D array with the data in the file position i, i.e. [][i][][].
    int error1 =0;
    InputData.resize(1);
    InputData[0].resize(i+1); //Setting proper size for AllData[0] 3D vector.

        QByteArray ba = fitxer.toLocal8Bit(); //convert fitxer into char c_str2
        const char *c_str2 = ba.data();
        ifstream obre(c_str2, ios::in);
        int m = 0;
        int p = 0;
        InputData[0][i].resize(ColNums(fitxer));
            for(int k=0;k<ColNums(fitxer);k++){                                                   // Run over all columns
                InputData[0][i][k].resize(RowNums(fitxer)+1); //+1 because RowNums counts the first line as 0.
            }

            //QMessageBox::information(this,"Information", QString("Number of cols is %1 and rows is %2") .arg(ColNums(WorkingFitxer)) .arg(RowNums(WorkingFitxer)+1) );

        std::string line;
            while (std::getline(obre, line)) {                  //Run over rows
                if(validate_row(line)){                         // It will ignore empty lines or lines with errors.
                    std::istringstream row(line);
                    std::string buffers;
                        while (row>>buffers){                           // Run over columns
                          InputData[0][i][p][m]=atof(buffers.c_str());
                         p++;
                        }
                    p =0;
                    m++;
                }else{
                    error1=1;
                }
            }
        obre.close();
        //QMessageBox::information(this,"Information", QString("Number of files is %1 and rows is %2") .arg(i) .arg(m) );

    if(error1==1){
      //QMessageBox::information(this,"Information", QString("Warning: Empty lines or lines with invalid data have been detected. Please make sure that the datafiles content exhibit proper format and numbers.") );
      return 1;
    }else{
      return 0;
    }
}

bool MainWindow::validate_double(const std::string& s) // Checks if a string is a double.
{
    double vardouble;
    return((std::istringstream(s) >> vardouble >> std::ws).eof());
}

bool MainWindow::validate_row(const std::string& s) // We consider that a row is valid (true) if all its elements are double.
{
    bool answer = true;
    std::istringstream row(s);
    std::string buffers;
    std::string check = s;

    if(!full_row(check)){
        answer = false;
        //QMessageBox::information(this,"Information", QString("Buffer has no data") );
    }
    while (row>>buffers){                                            // Run over columns
        if(!validate_double(buffers)){               //We check if all values are double or rows are empty.
            answer = false;
            //QMessageBox::information(this,"Information", QString("Buffer is not a double") );
        }
    }
   return(answer);
}

bool MainWindow::full_row(const std::string& check) //Checks if a row has data (true) or is empty (false)
{
    bool answer = false;
    std::istringstream row(check);
    std::string buffers;
    double buffer2;
    while (row>>buffers){                           // Run over columns
        if(validate_double(buffers)){             //We check that all input strings are indeed double numbers.
            buffer2 = atof(buffers.c_str());
            //QMessageBox::information(this,"Information", QString("This: %1 has been validated as a double") .arg(buffer2) );
            answer = true;
        }
    }
    //QMessageBox::information(this,"Information", QString("FullRow returns: %1") .arg(answer) );
   return(answer);
}

void MainWindow::findColRowNums(QStringList fitxer){

    FileColNums.clear();
    FileRowNums.clear();
    FileColNums.resize(3);
    FileRowNums.resize(3);

    FileColNums[0]=0; // No error, since there is only one file.
    FileRowNums[0]=0;
    FileColNums[1]=ColNums(fitxer.at(0)); // Col Min
    FileRowNums[1]=RowNums(fitxer.at(0)); // Row Min
    FileColNums[2]=ColNums(fitxer.at(0)); // Col Max
    FileRowNums[2]=RowNums(fitxer.at(0)); // Row Max

    if(fitxer.size()>0){
        for (int q=1; q<fitxer.size();q++){     // run over files
            if(ColNums(fitxer.at(q))!=FileColNums[1]){
                FileColNums[0]=1; // Error detected, different nums of cols for different files.
                if(ColNums(fitxer.at(q))<FileColNums[1]){
                    FileColNums[1]=ColNums(fitxer.at(q));
                }else if (ColNums(fitxer.at(q))>FileColNums[2]){
                    FileColNums[2]=ColNums(fitxer.at(q));
                }
            }
            if(RowNums(fitxer.at(q))!=FileRowNums[1]){
                FileRowNums[0]=1; // Error detected, different nums of rows for different files.
                if(RowNums(fitxer.at(q))<FileRowNums[1]){
                    FileRowNums[1]=RowNums(fitxer.at(q));
                }else if (RowNums(fitxer.at(q))>FileRowNums[2]){
                    FileRowNums[2]=RowNums(fitxer.at(q));
                }
            }
        }
    }
}

int MainWindow::ColNums(QString fitxer){ //Returns number of columns for a datafile by reading the first valid line.
    //QMessageBox::information(this,"Information", QString("I Access ColNums") );
    ColumnSize =0;
    QByteArray ba = fitxer.toLocal8Bit(); //convert fitxer into char c_str2
      const char *c_str2 = ba.data();
      ifstream obre(c_str2, ios::in);
            std::string myline;
            while(std::getline(obre, myline)){      //Finds first non-empty row with only doubles in it.
                //QMessageBox::information(this,"Information", QString("I got a line") );
                if(validate_row(myline)){
                    //QMessageBox::information(this,"Information", QString("I break loop now") );
                    break;
                }
            }
            std::istringstream row(myline);
            double whatever;
            while (row>>whatever){
                ColumnSize++;
            }
        obre.close();
return ColumnSize;
}

int MainWindow::RowNums(QString fitxer){
    int num = 0;
    QByteArray ba = fitxer.toLocal8Bit(); //convert fitxer into char c_str2
      const char *c_str2 = ba.data();
    ifstream ifs(c_str2, ios::in); // Input
        std::string line;
        while (std::getline(ifs, line)) {                  //Run over rows
            if(validate_row(line)){                         // It will ignore empty lines or lines with errors.
             num++;
            }else{
                //QMessageBox::information(this, "Information for debug", QString("Line %1 has an error") .arg(num) );
            }
          }
    ifs.close();
    RowSize=num-1;
    return RowSize;
}

int MainWindow::ColorFunc(int i, double p){
    double theta = 1.0*i*PI/6+p*PI;
    double result = 150*(sin(theta)+(sin(3*theta))/3+(sin(5*theta))/5+(sin(7*theta))/7+sin((9*theta))/9)+255/2; //Squared function with phase p
    int y = (int) result;
    return y;
}

void MainWindow::SwapCols(QVector<QVector<double>> &vec, int result1, int result2){
    double temp1;
    for(int q=0; q<vec[0].size();q++){ //run over rows.
        temp1=vec[result1-1][q];
        vec[result1-1][q]=vec[result2-1][q];
        vec[result2-1][q]=temp1;
    }
}

void MainWindow::DeleteCol(QVector<QVector<double>> &vec, int result){
int e =0;
    for(int q=0;q<vec.length();q++){                // Run over columns
        if((result-1)!=q){                          // Skips col to be deleted
            for(int w=0; w<vec[q].length();w++){    // Run over rows
                vec[e][w]=vec[q][w];
            }
            e++;
        }
    }
    vec.resize(vec.size()-1);
}

double MainWindow::getMyDouble(QString message){
    QString text;
    bool error1 = 0;
    bool ok;
    /*
    QString text = QInputDialog::getText(this, tr("QInputDialog::getText()"),
                                         tr("User name:"), QLineEdit::Normal, QDir::home().dirName(), &ok);
    */
    while(!error1){
        text = QInputDialog::getText(this, "Input", message,QLineEdit::Normal,"",&ok);
        if(ok){
            if(text != "0"){
                error1=text.toDouble();
                if(!error1){
                    QMessageBox::warning(this, "Error", QString("No valid number was introduced.\nExemples of valid numbers are: 5, -1.59e-29, 0, 300.2.\nPlease, try again."));
                }
            }else{
                error1=1;
            }
        }else{
            cancell = true;
            return 0;
        }
    }
    return text.toDouble();
}

//*********FILE FUNCTIONS***************

void MainWindow::SaveAll(QStringList fitxers, QStringList fitxersSavedAt, QVector<QVector<int>> Actions){
    QVector<QVector<QVector<QVector<double>>>> TemporalArr;
    int error =0;
    for(int q=0; q<fitxers.size(); q++){            // Run over all files
        TemporalArr.resize(1);
        error += getOneData(fitxers.at(q), TemporalArr);     // Get TemporalArr[0][0][col][rows] IDEALLY should call getData, but it was faster to make a new function.
        int TempCol =2;                             // Working column, default 1.
        int TempColRef = 2;                         // Reference column, default 1.

        for(int w=0;w<Actions.size();w++){          // Run over all actions
            int y = (int) Actions[w][0];            // Maybe the (int) was redundant since i think at the beggining actions was a double.
            switch (y) {
                case 1:{
                    TempCol = Actions[w][1];
                break;
                }
                case 2:{
                    AddConstant(TemporalArr[0][0],AllActions[w][1],RememberInv[w][0]);
                break;
                }
                case 3:{
                    TempColRef = Actions[w][1];
                break;
                }
                case 4:{
                    SubtractColumns(TemporalArr[0][0], AllActions[w][1], AllActions[w][2]);
                break;
                }
                case 5:{
                    DivideColumns(TemporalArr[0][0], AllActions[w][1], AllActions[w][2]);
                break;
                }
                case 6:{
                    MultiplyColumns(TemporalArr[0][0], AllActions[w][1], AllActions[w][2]);
                break;
                }
                case 7:{
                    DeleteCol(TemporalArr[0][0], AllActions[w][1]);
                break;
                }
                case 8:{
                    SwapCols(TemporalArr[0][0], AllActions[w][1], AllActions[w][2]);
                break;
                }
                case 9:{
                     Trim(TemporalArr[0][0], RememberInv[w][0], RememberInv[w][1]);
                break;
                }
                case 10:{
                     LinearBaseline(TemporalArr[0][0], AllActions[w][1]);
                break;
                }
                case 11:{
                    MultConsty(TemporalArr[0][0], RememberInv[w][q], AllActions[w][1]);
                break;
                }
                case 12:{
                    MultConsty(TemporalArr[0][0], RememberInv[w][q], AllActions[w][1]);
                break;
                }
                case 13:{
                    MultConsty(TemporalArr[0][0], RememberInv[w][q], AllActions[w][1]);
                break;
                }
                case 14:{
                    MultConsty(TemporalArr[0][0], RememberInv[w][q], AllActions[w][1]);
                break;
                }
                case 15:{
                    MultConsty(TemporalArr[0][0], RememberInv[w][0], AllActions[w][1]);
                break;
                }
                case 16:{
                    spikinator(TemporalArr[0][0], AllActions[w][1], AllActions[w][2], RememberInv[w][0], 1);
                break;
                }
                case 17:{
                    VerticalStack(TemporalArr[0][0], q, RememberInv[w][0]);
                break;
                }
                case 18:{
                    BallBaseline(TemporalArr[0][0], AllActions[w][1], RememberInv[w][0]);
                break;
                }
                case 19:{
                    AddColumns(TemporalArr[0][0], AllActions[w][1], AllActions[w][2]);
                break;
                }
                case 20:{
                    Filter(TemporalArr[0][0], AllActions[w][1], RememberInv[w][0], RememberInv[w][1]);
                break;
                }
                case 21:{
                    KKA(TemporalArr[0][0], AllActions[w][2]);
                break;
                }
                case 22:{
                    DuplicateCol(TemporalArr[0][0], AllActions[w][1]);
                break;
                }


//                AllActions[AllActions.size()-1].resize(1);
//                AllActions[AllActions.size()-1][0]=17;
//                RememberInv[RememberInv.size()-1][0]=VerticalShift

            }
        }

        //SAVE FILE HERE***********************************************************************************************
        SaveData(fitxersSavedAt.at(q), TemporalArr);
        TemporalArr.clear();                        // TemporalArr is reset.
    }
    if (error >=1){
        QMessageBox::information(this,"Information", QString("Warning: Empty lines or lines with invalid data have been detected in %1 files. Saved results automatically corrected some of the errors in the files.") .arg(error) );
    }
}

void MainWindow::SaveData(QString fitxer, QVector<QVector<QVector<QVector<double>>>> &data){ //Note!! It only saves from first datafile! [0][0]!

        QByteArray ba = fitxer.toLocal8Bit(); //convert fitxer into char c_str2
        const char *c_str2 = ba.data();
        ofstream ofs(c_str2, ofstream::trunc); // trunc is used to overwritte file.
        //ofs.precision(dbl::max_digits10);
        for (int r=0; r<data[0][0][0].size()-1;r++){  //Run over rows
            for(int e =0; e<data[0][0].size()-1;e++){ //Run over columns
                    ofs << std::scientific << data[0][0][e][r] << '\t';
                }
                ofs << std::scientific << data[0][0][data[0][0].size()-1][r]<< '\n'; // We do this to avoid a redundant tab, and also insert enter.
            }
        for(int e =0; e<data[0][0].size()-1;e++){ //We do this to avoid last enter in the file.
            ofs << std::scientific << data[0][0][e][data[0][0][0].size()-1] << '\t';
        }
            ofs << std::scientific << data[0][0][data[0][0].size()-1][data[0][0][0].size()-1];
        ofs.close();
}

void MainWindow::SaveSingleData(QString fitxer, QVector<QVector<double>> &data){ //Note!! It only saves from first datafile! [0][0]!

        QByteArray ba = fitxer.toLocal8Bit(); //convert fitxer into char c_str2
        const char *c_str2 = ba.data();
        ofstream ofs(c_str2, ofstream::trunc); // trunc is used to overwritte file.
        //ofs.precision(dbl::max_digits10);
        for (int r=0; r<data[0].size()-1;r++){  //Run over rows
            for(int e =0; e<data.size()-1;e++){ //Run over columns
                    ofs << std::scientific << data[e][r] << '\t';
                }
                ofs << std::scientific << data[data.size()-1][r]<< '\n'; // We do this to avoid a redundant tab, and also insert enter.
            }
        for(int e =0; e<data.size()-1;e++){ //We do this to avoid last enter in the file.
            ofs << std::scientific << data[e][data[0].size()-1] << '\t';
        }
            ofs << std::scientific << data[data.size()-1][data[0].size()-1];
        ofs.close();
}

//********* FILES AND COLUMNS ***************

void MainWindow::DuplicateCol(QVector<QVector<double>> &vec, int result){
    vec.resize(vec.size()+1);               // We add one column.
    vec[vec.size()-1].resize(vec[0].size());

    for(int q=0; q<vec[0].size();q++){      // Run over rows.
        vec[vec.size()-1][q]=vec[result-1][q];
    }
}

void MainWindow::findMinCol(QVector<QVector<QVector<double>>> &vec){
    ColumnMinSize = vec[0].size();                      // number of columns of first file.
    for(int q =0; q<vec.size();q++){                    // run over files.
        if(vec[q].size()<ColumnMinSize){
            ColumnMinSize=vec[q].size();
        }
    }
}

//*********ANALYSIS FUNCTIONS***************

void MainWindow::Filter(QVector<QVector<double>> &a1, int Col, double lowfreq, double highfreq){

    int lines=a1[0].size();
    //QMessageBox::information(this, "Information", QString("Lines are %1") .arg(lines) );
    complex<double> a[2][lines];
    complex<double> b[lines];

     for(int i=0;i<lines;i++){
     b[i]=0;
     a[0][i]=a1[0][i];
     a[1][i]=a1[Col-1][i];
     }

     for(int w = 0; w<lines; w++){ //We make the Fourier transform
        for(int i=0; i<lines;i++){
            b[w] =b[w]+(1/pow(lines,0.5))*a[1][i]*pow(N_E, std::complex<double>(0,-2*PI*w*i/lines));
        }
     }

    for(int i =0;i<lines/2;i++){ //We delete the interference
        double w = i/(abs(real(a[0][lines-1])-real(a[0][0])));
        if(w <= highfreq && w >= lowfreq){
        b[i]=0;
        }
    }

    int j = 0;
    for(int i =lines/2;i<lines;i++){
        double w = j/abs(real(a[0][lines-1])-real(a[0][0])) - lines/(2*abs(real(a[0][lines-1])-real(a[0][0])));
        if(w <= -lowfreq && w >= -highfreq){
        b[i]=0;
        }
        j = j+1;
    }

    complex<double> c[lines]; //We undo the Fourier transform
    for(int i=0;i<lines;i++){
    c[i]=0;
    }
    for(int i = 0; i<lines; i++){
        for(int w=0; w<lines;w++){
            c[i] =c[i]+(1/pow(lines,0.5))*b[w]*pow(N_E, std::complex<double>(0,2*PI*w*i/lines));
        }
    }

    for(int f=0; f<lines; f++){ // We writte the result
        a1[0][f]= real(a[0][f]);
        a1[Col-1][f]= real(c[f]);
        //a1[Col-1][f]= real(b[f]);
    }
}

void MainWindow::AddConstant(QVector<QVector<double>> &vec,int colPlot, double result){
    for (int w = 0; w < vec[0].length(); w++){
        vec[colPlot-1][w] = vec[colPlot-1][w]+result;
    }
}

void MainWindow::KKA(QVector<QVector<double>> &vec, int columnPlot){
//    vec.resize(vec.size()+1);                                   //We add one column for the answer. On secon thought, I think it is better to overwritte the files, like in Filter.
//    vec[vec.size()-1].resize(vec[0].size());                    //We create space for all rows in new column.
    QVector<QVector<double>> vecTemp; //temporal vector
    vecTemp.resize(1);                                   //We add one column for the answer. On secon thought, I think it is better to overwritte the files, like in Filter.
    vecTemp[0].resize(vec[0].size());


    double integral;
    double deltaE = abs(vec[0][0]-vec[0][1]);
    for(int q=0; q<vec[0].size();q++){
        integral = 0.0;
        for(int w=0; w<vec[0].size();w++){
            if(q!=w){
                integral = integral+2*vec[0][q]*vec[columnPlot-1][w]*deltaE/(PI*(pow(vec[0][q],2)-pow(vec[0][w],2)));
            }
        }
        //vec[vec.size()-1][q]=pow(pow(vec[columnPlot-1][q],2)+pow(integral,2),0.5);
        vecTemp[0][q]=pow(pow(vec[columnPlot-1][q],2)+pow(integral,2),0.5);
    }    
    for(int e=0; e<vec[0].size();e++){
        vec[columnPlot-1][e] = vecTemp[0][e];
    }
}

void MainWindow::SubtractColumns(QVector<QVector<double>> &vec,int colPlot,int RefCol){

    vec.resize(vec.size()+1);                                   //We add one column for the answer
    vec[vec.size()-1].resize(vec[0].size());                    //We create space for all rows in new column.

    for(int q=0;q<vec[0].size();q++){                           //Run over rows
        vec[vec.size()-1][q]=vec[colPlot-1][q]-vec[RefCol-1][q];  //Subtracting columns
    }
}

void MainWindow::AddColumns(QVector<QVector<double>> &vec,int colPlot,int RefCol){

    vec.resize(vec.size()+1);                                   //We add one column for the answer
    vec[vec.size()-1].resize(vec[0].size());                    //We create space for all rows in new column.

    for(int q=0;q<vec[0].size();q++){                           //Run over rows
        vec[vec.size()-1][q]=vec[colPlot-1][q]+vec[RefCol-1][q];  //Adding columns
    }
}

void MainWindow::DivideColumns(QVector<QVector<double>> &vec,int colPlot,int RefCol){
    bool warning = false;
    vec.resize(vec.size()+1);                                   //We add one column for the answer
    vec[vec.size()-1].resize(vec[0].size());                    //We create space for all rows in new column.

    for(int q=0;q<vec[0].size();q++){                           //Run over rows
        if(vec[RefCol-1][q]!=0){
            vec[vec.size()-1][q]=vec[colPlot-1][q]/vec[RefCol-1][q];  //Dividing columns
        }else if (vec[RefCol-1][q]==0){
            vec[vec.size()-1][q]=0;
            warning = true;
        }
    }
    if(warning==true){
        QMessageBox::information(this, "Warning!", QString("Some denominator values equal 0. Since cannot divide by 0, result is set to 0.") );
    }
}

void MainWindow::MultiplyColumns(QVector<QVector<double>> &vec,int colPlot,int RefCol){
    vec.resize(vec.size()+1);                                   //We add one column for the answer
    vec[vec.size()-1].resize(vec[0].size());                    //We create space for all rows in new column.
    for(int q=0;q<vec[0].size();q++){                           //Run over rows
      vec[vec.size()-1][q]=vec[colPlot-1][q]*vec[RefCol-1][q];  //Multiplying columns
    }
}

void MainWindow::MultConsty(QVector<QVector<double> > &vec, double value, int col){
    for(int q=0;q<vec[col-1].size();q++){
        vec[col-1][q]=vec[col-1][q]*value;
    }
}

//*********PREPARE DATA***************

double MainWindow::FindMin(QVector<QVector<double>> &vec){
    double min =0;

    if(vec[0][vec[0].size()-1]>vec[0][0]){
        min = vec[0][0];
    }else{
        min = vec[0][vec[0].size()-1];
    }
    return min;
}

double MainWindow::FindMax(QVector<QVector<double>> &vec){
    double max =0;

    if(vec[0][vec[0].size()-1]>vec[0][0]){
        max = vec[0][vec[0].size()-1];
    }else{
        max = vec[0][0];
    }
    return max;
}

double MainWindow::FindMinFile(QString fitxer){
    double min =0;
    QVector<QVector<double>> vec1;
    getOneSingleData(fitxer, vec1);
    if(vec1[0][vec1[0].size()-1]>vec1[0][0]){
        min = vec1[0][0];
    }else{
        min = vec1[0][vec1[0].size()-1];
    }
    return min;
}

double MainWindow::FindMaxFile(QString fitxer){
    double max =0;
    QVector<QVector<double>> vec1;
    getOneSingleData(fitxer, vec1);
    if(vec1[0][vec1[0].size()-1]>vec1[0][0]){
        max = vec1[0][vec1[0].size()-1];
    }else{
        max = vec1[0][0];
    }
    return max;
}

void MainWindow::BallBaseline(QVector<QVector<double>> &vec, int col, double radii){ //This program wont work for small datavalues with Y below 0.1.
    // Important variables: cenx and ceny are the position of the ball's center.
    // xt and yt are the position of a datapoint relative to the ball's center.
    // yballatt is the vertical position of the circle at an x corresponding to a xdatapoint at a certain index t.
    // the position of the datapoints is easily found in vec[][].

    //We find the max and min of the spectra in the y axis.
    int lines = vec[0].size();
    double pas = fabs((vec[0][lines-1]-vec[0][0])/(lines-1));			// increment x
    int inv; // check whether the X data is inverted in order.
    double minx, maxx; // min and max values of Xi.
    double xt, yt, yatminx; // Difference x position between ball center and xi.
    QVector<double> baseline;
    baseline.resize(vec[0].size());

    double globalminY = vec[col-1][0]; //We search for the global minimum of the spectra
    double globalmaxY = vec[col-1][0]; //We search for the global max of the spectra

    for(int j = 0; j < lines;j++){
     if(vec[col-1][j]<globalminY){
     globalminY = vec[col-1][j];
     }else if(vec[col-1][j]>globalmaxY){
         globalmaxY = vec[col-1][j];
     }
    }

    for (int i=0; i<lines;i++){ //The baseline is set at the global minimum, and will be increased by the rolling ball.
    baseline[i]=globalminY;
    }

    if(vec[0][0]<vec[0][lines-1]){
        minx = vec[0][0];
        maxx = vec[0][lines-1];
        yatminx = vec[col-1][0];
        inv = 1;
    }else{
        minx = vec[0][lines-1];
        maxx = vec[0][0];
        yatminx = vec[col-1][lines-1];
        inv =-1;
    }
    // factor is used to re-scale the Y dimension, which in general will be different than the X dimension in size.
    // The ball fitting method won't work unless both dimensions are compensated and in the same scale.
        double factor = (globalmaxY-globalminY)/(maxx-minx);
        for(int m=0; m<lines;m++){
            vec[col-1][m]=vec[col-1][m]/factor;
        }
         globalminY = globalminY/factor;
         globalmaxY = globalmaxY/factor;

         for (int i=0; i<lines;i++){ //The baseline is set at the global minimum, and will be increased by the rolling ball.
         baseline[i]=globalminY;
         }

         if(vec[0][0]<vec[0][lines-1]){
             minx = vec[0][0];
             maxx = vec[0][lines-1];
             yatminx = vec[col-1][0];
             inv = 1;
         }else{
             minx = vec[0][lines-1];
             maxx = vec[0][0];
             yatminx = vec[col-1][lines-1];
             inv =-1;
         }
      //Note for the programmers: There is a better way to apply the factor shit which does not imply reescaling the whole spectra
      //At the beggining (lines above) and then reescaling the whole spectra at the end (see end of function). The smarter way
      //is to incorporate it inside the lines around "yballatt = ceny+ pow(pow(radii,2)-pow(xt,2),0.5);" (see code in the middle).
      //and use a differential-like method rather than absolute method. However I was not able to figure it out. See my previous
      //spikinator.exe program to understand the optimized method (caution: that code works only under certain conditions).
      //The current code, although inefficient, works well.

    // Here the ball runs over the X axis, repositioning its vertical position ceny as it advances cenx.
    // Each time the ball has moved rightward and repositioned vertically, the baseline is updated.
    // The code will work regardless the X data is increasing or decreasing with increasing row number.
    double ceny, yballatt;
    int t;
    for(double cenx=(minx-radii); cenx<=(maxx+radii); cenx+=pas){ // The ball runs over X values, centered at cenx.
        //xt = fabs(minx-cenx);						//We set the first y position of the ball. (i=0 from loop below)
        //ycircle = pow(pow(radii,2)-pow(xt,2),0.5);	//We set the first y position of the ball. (i=0 from loop below)
        //maxdify = yatminx - ycircle;				//We set the first y position of the ball. (i=0 from loop below)

        ceny=globalmaxY;   //we initialize ceny at a place that is for sure higher than some point, in the next step it will go down.
        for(int i=0;i<lines;i++){ // run over Xi to find maxdify for the current ball position.
         t = i;
         if(inv == -1){	//we redefine int i for tha case data ordering is inverted.
             t= lines-i-1;
         }
         //QMessageBox::information(this, "Information", QString("index t is %1 corresponds to x= %2 and y= %3") .arg(t) .arg(vec[0][t]) .arg(vec[col-1][t]) );
            xt = fabs(vec[0][t]-cenx); // distance ball center to point xi.
            yt = vec[col-1][t]-ceny;

         if(xt<=radii){ // we only need to check the yi data in the range of the ball.
             yballatt=pow(pow(radii,2)-pow(xt,2),0.5)+ceny; //y value of the circle at t point, in absolute terms.
            if(yballatt>vec[col-1][t]){ //must be checked in absolute terms
                ceny=ceny-(yballatt-vec[col-1][t]); //We bring the ball to a lower position.
            }
         }
        }
       // QMessageBox::information(this, "Information", QString("Working") );

        //Now we know the position of the ball in its center, cenx ceny. We must update the baseline vector bringing it up.
        for(int j=0;j<lines;j++){
         int r = j;
         if(inv == -1){	//we redefine int i for tha case data ordering is inverted.
             r= lines-j-1;
         }
         xt = fabs(vec[0][r]-cenx); // distance ball center to point xi.
         if(xt<=radii){ // we only need to check the yi data in the range of the ball.
            yballatt = ceny+ pow(pow(radii,2)-pow(xt,2),0.5);
             if(baseline[r]< yballatt ){ // We update the baseline, raising it as much as possible (it was defined at globalminY).
                 baseline[r] = yballatt;
             }
         }
        }
        //QMessageBox::information(this, "Information", QString("Working at cenx = %1") .arg(cenx) );
    }

    for(int q=0; q<lines;q++){ //run over rows
        int s = q;
        if(inv == -1){	//we redefine int q for tha case data ordering is inverted.
         s= lines-q-1;
        }
        vec[col-1][s]=vec[col-1][s]-baseline[s];
    }

    //Here we undo the factor shit
    for(int m=0; m<lines;m++){
        vec[col-1][m]=vec[col-1][m]*factor;
    }


}

double MainWindow::Integrate(QVector<QVector<double>> &vec, int col){
    double result=0;
    for(int q=0; q<vec[col-1].size()-1; q++){  // run over rows
        result=result+fabs((vec[0][1]-vec[0][0])*(vec[col-1][q]+vec[col-1][q+1])/2);
    }
return result;
}

double MainWindow::Maxy(QVector<QVector<double>> &vec, int col){
    double result=vec[col-1][0];
    for(int q=0; q<vec[col-1].size(); q++){  // run over rows
        if(result<vec[col-1][q]){
            result=vec[col-1][q];
        }
    }
return result;
}

void MainWindow::LinearBaseline(QVector<QVector<double>> &vec, int col){
    double slope = (vec[col-1][vec[col-1].size()-1]-vec[col-1][0])/(vec[0][vec[col-1].size()-1]-vec[0][0]);
    double intercept = vec[col-1][0]-slope*vec[0][0];
    for(int q=0; q<vec[col-1].size();q++){
        vec[col-1][q]=vec[col-1][q]-intercept-slope*vec[0][q];
    }
}

void MainWindow::Trim(QVector<QVector<double>> &vec, double result1, double result2){
int a=0;
QVector<QVector<double>> temp;
temp.resize(vec.size());
        for (int a= 0; a<vec.size();a++){                        //run over cols
            temp[a].resize(vec[a].size());
        }
        for (int q= 0; q<vec[0].size();q++){                        //run over rows
            if((vec[0][q]>=result1) && (vec[0][q]<=result2) ){
                for(int w =0; w<vec.size();w++){                    //run over columns
                    temp[w][a]=vec[w][q];
                }
                a++;
            }
        }

        for (int e= 0; e<temp.size();e++){                        //run over cols
                for(int r =0; r<temp[e].size();r++){              //run over rows
                    vec[e][r]=temp[e][r];
                }
        }

        for(int w =0; w<vec.size();w++){
            vec[w].resize(a);
        }
temp.clear();
}

double MainWindow::GenerateRandom(double min, double max){
    double answer = (max - min) * ( (double)rand() / (double)RAND_MAX ) + min;
    return answer;
}

int MainWindow::spikinator(QVector<QVector<double>> &vec, int points, int col, double sigmas, int modify){
    QVector<double> result;
    int i=0;
    result.resize(3);//first for A, second for B, y=A+B*x, third for standard deviation SD
    double expectedyval =0;
        for(int q=0; q<vec[col-1].size()-points; q++){ // run over rows
        linearfit(vec, result, points, q, col);
        SD(vec, result, points, q, col);
        expectedyval = expectedy(vec, result, points, q);
            if(((vec[col-1][q+points])>(expectedyval+sigmas*result[2])) || ((vec[col-1][q+points])<(expectedyval-sigmas*result[2]))){ //if so, point must be corrected
               if(modify==1){
                   vec[col-1][q+points]=expectedyval+1.8*result[2]*GenerateRandom(-1.0,1.0); // new code more random than lines below.
               }
                i++;
            }
        }

        //We want to run backwards to detect spikes at the beggining of the code within the first "points".
        QVector<QVector<double>> Inversevec;
        Inversevec.resize(vec.size());
        for(int w=0;w<vec.size();w++){
            Inversevec[w].resize(vec[w].size());
            for(int e =0; e<vec[w].size();e++){
                Inversevec[w][vec[w].size()-1-e]=vec[w][e];
            }
        }
        expectedyval =0;

        for(int q=0; q<Inversevec[col-1].size()-points; q++){ //We run backwards
        linearfit(Inversevec, result, points, q, col);
        SD(Inversevec, result, points, q, col);
        expectedyval = expectedy(Inversevec, result, points, q);
            if(((Inversevec[col-1][q+points])>(expectedyval+sigmas*result[2])) || ((Inversevec[col-1][q+points])<(expectedyval-sigmas*result[2]))){ //if so, point must be corrected
               if(modify==1){
                   Inversevec[col-1][q+points]=expectedyval+1.8*result[2]*GenerateRandom(-1.0,1.0);
               }
                i++;
            }
        }
        //We recover data from funning forward and backward.
        for(int w=0;w<vec.size();w++){
            for(int e =0; e<vec[w].size();e++){
                vec[w][vec[w].size()-1-e]=Inversevec[w][e];
            }
        }

    return i; // number of spikes corrected.
}

 void MainWindow::linearfit(QVector<QVector<double>> vec, QVector<double> &result, int points, int firstpoint, int col){
    //Makes a linear fit and sends A, B for y=A+B*x
     double sumx=0, sumy=0, sumxx=0, sumxy=0;
     for(int q= 0; q<points; q++){
         sumx = sumx+vec[0][q+firstpoint];
         sumy = sumy+vec[col-1][q+firstpoint];
         sumxx = sumxx+pow(vec[0][q+firstpoint],2);
         sumxy = sumxy+vec[0][q+firstpoint]*vec[col-1][q+firstpoint];
     }
     result[0]=((sumy*sumxx)-(sumx*sumxy))/((points*sumxx)-pow(sumx,2));   // A intercept
     result[1]=((points*sumxy)-(sumx*sumy))/((points*sumxx)-pow(sumx,2));  // B slope
}

 double MainWindow::expectedy(QVector<QVector<double>> vec, QVector<double> result, int points, int firstpoint){
     //Returns expected y value given A, B for y=A+B*x.
     return (result[0]+result[1]*vec[0][firstpoint+points]);
 }

 void MainWindow::SD(QVector<QVector<double>> vec, QVector<double> &result, int points, int firstpoint, int col){
     //Returns standard deviation from previous points and linear fit values.
     double sigma=0, sum=0;
     for(int q= 0; q<points; q++){
         sum=sum+pow(vec[col-1][q+firstpoint]-result[0]-result[1]*vec[0][q+firstpoint],2);
     }
     sigma =pow(sum/(points-1),0.5);
     result[2]=sigma;
 }

//*********GRAPH FUNCTIONS***************

 void MainWindow::VerticalStack(QVector<QVector<double>> &vec, int q, double VerticalShift){
         for(int w=0; w<vec[columnPlot-1].size(); w++){
             vec[columnPlot-1][w]=vec[columnPlot-1][w]+q*VerticalShift;
         }
 }

void MainWindow::rescale_rob(int flag){  // rescale axis
    double margin = 10; // percentage of margin
    if (plotNum>=1){

      //  QMessageBox::information(this,"AllData[0] exists", QString("Data starts x= %1 , y= %2") .arg(AllData[0][0][1]) .arg(AllData[0][1][1]));
        double* min_x = std::min_element(AllData[0][0][0].begin(), AllData[0][0][0].end());
        double* max_x = std::max_element(AllData[0][0][0].begin(), AllData[0][0][0].end());
        double* min_y = std::min_element(AllData[0][0][columnPlot-1].begin(), AllData[0][0][columnPlot-1].end());
        double* max_y = std::max_element(AllData[0][0][columnPlot-1].begin(), AllData[0][0][columnPlot-1].end());
        double min_abs_x=*min_x;
        double max_abs_x=*max_x;
        double min_abs_y=*min_y;
        double max_abs_y=*max_y;
        // if (ui->plot->selectedGraphs().size() > 1){
        //QMessageBox::information(this,"Initial data", QString("x from %1 to %2 and y from %3 to %4") .arg(min_abs_x) .arg(max_abs_x) .arg(min_abs_y) .arg(max_abs_y));
         if (flag == 1){
            for (int i = 1; i< plotNum;i++){
                 //QMessageBox::information(this,"Rescaling", QString("Loop x= %1") .arg(i));
                double* min_x = std::min_element(AllData[0][i][0].begin(), AllData[0][i][0].end());
                double* max_x = std::max_element(AllData[0][i][0].begin(), AllData[0][i][0].end());
                double* min_y = std::min_element(AllData[0][i][columnPlot-1].begin(), AllData[0][i][columnPlot-1].end());
                double* max_y = std::max_element(AllData[0][i][columnPlot-1].begin(), AllData[0][i][columnPlot-1].end());
                 if (*min_x<min_abs_x){
                     min_abs_x=*min_x;
                 }
                 if (*min_y<min_abs_y){
                     min_abs_y=*min_y;
                 }
                 if (*max_x>max_abs_x){
                     max_abs_x=*max_x;
                 }
                 if (*max_y>max_abs_y){
                     max_abs_y=*max_y;
                 }
                 //QMessageBox::information(this,"AllData[0] exists", QString("Spectra num. %1 goes from y= %2 to y= %3") .arg(i) .arg(*min_y) .arg(*max_y));
                 //QMessageBox::information(this,"AllData[0] exists", QString("Spectra num. %1, rescaled from y= %2 to y= %3") .arg(i) .arg(min_abs_y) .arg(max_abs_y));
             }
         }

        double mean_x = (max_abs_x-min_abs_x);
        double mean_y = (max_abs_y-min_abs_y);
        double xlow, xhigh, ylow, yhigh;
        if(log){    //If log scale is selected....
            xlow = min_abs_x - (margin/100)*mean_x;
            xhigh = max_abs_x + (margin/100)*mean_x;
            ylow = pow(10, log10(min_abs_y) - fabs((margin/100)*log10(mean_y)));
            yhigh = pow(10, log10(max_abs_y) + fabs((margin/100)*log10(mean_y)));
        }else{
            xlow = min_abs_x - (margin/100)*mean_x;
            xhigh = max_abs_x + (margin/100)*mean_x;
            ylow = min_abs_y - (margin/100)*mean_y;
            yhigh = max_abs_y + (margin/100)*mean_y;
        }
        if(xlow==xhigh){
            ui->plot->xAxis->setRange(xlow-1, xhigh+1);
            ui->plot->yAxis->setRange(ylow, yhigh);
        }else if(ylow==yhigh){
            ui->plot->xAxis->setRange(xlow, xhigh);
            ui->plot->yAxis->setRange(ylow-1, yhigh+1);
        }else{
            ui->plot->xAxis->setRange(xlow, xhigh);
            ui->plot->yAxis->setRange(ylow, yhigh);
        }
    }
}

void MainWindow::plotall(int q){
    ui->plot->clearGraphs();
    QStringList pieces;
    switch(q){
        case 0:{ //Only plot one spectrum
            ui->plot->addGraph();
            ui->plot->graph(0)->setData(AllData[0][0][0],AllData[0][0][columnPlot-1]);
            flag1 = 0;
            rescale_rob(flag1);
            QPen myPen;
            myPen.setWidth(2);
            ui->plot->graph()->setPen(myPen);
            pieces = filenames.at(0).split("/");
            pieces = pieces.at(pieces.length()-1).split(".");
            ui->plot->graph()->setName(pieces.at( pieces.length()-2));
            ui->plot->replot();
        break;
        }
        case 1:{ //Plot all spectra
            //Here I need to load all data to AllData[0]!!!
            //QVector<double> tempShift;
            for (int i = 0; i< plotNum;i++){
                ui->plot->addGraph();
                ui->plot->graph(i)->setData(AllData[0][i][0],AllData[0][i][columnPlot-1]);
                pieces = filenames.at(i).split("/");
                pieces = pieces.at(pieces.length()-1).split(".");
                ui->plot->graph()->setName(pieces.at( pieces.length()-2));
                QPen graphPen;
                 graphPen.setColor(RGB(ColorFunc(i,0.0), ColorFunc(i,-0.666667), ColorFunc(i,0.666667)));
                 graphPen.setWidth(2);
                 ui->plot->graph()->setPen(graphPen);
                 ui->plot->replot();
                 ui->plot->update();
            }
            flag1 = 1;
            rescale_rob(flag1);
            ui->plot->replot();
            ui->plot->update();
        break;
        }
    }
}

//*********GRAPH:UNDO FUNCTIONS**********

void MainWindow::Backup(){
    for(int m=3; m>=0;m--){                                                 // run over backups
        if(!AllData[m].empty()){
        AllData[m+1].resize(AllData[m].size());
            for(int p=0;p<AllData[m].size();p++){                           // run over files
            AllData[m+1][p].resize(AllData[m][p].size());
                for(int k=0;k<AllData[m][p].size();k++){                    // run over columns
                AllData[m+1][p][k].resize(AllData[m][p][k].size());
                    for(int r = 0; r<AllData[m][p][k].size();r++){          // run over rows
                        AllData[m+1][p][k][r]=AllData[m][p][k][r];
                    }
                }
            }
        }
    }
}

void MainWindow::GetBackupFunc(){
    for(int m=0; m<4;m++){
        if(!AllData[m+1].empty()){
            AllData[m].resize(AllData[m+1].size());
               for(int p=0;p<AllData[m].size();p++){
                AllData[m][p].resize(AllData[m+1][p].size());
                for(int k=0;k<AllData[m][p].size();k++){
                    AllData[m][p][k].resize(AllData[m+1][p][k].size());
                        for(int r=0;r<AllData[m][p][k].size();r++){
                            AllData[m][p][k][r]=AllData[m+1][p][k][r];
                        }
                }
               }
        }
    }
}

/*
void MainWindow::InputAction(int action, QVector<double> &inputs){
    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(inputs.size());
    for(int q=0; q<inputs.size();q++){
        AllActions[AllActions.size()-1][q]=inputs[q];
    }
}*/

void MainWindow::GetBackup(){
    //for(int m=3; m>=0;m--){
 if(AllActions.size()!=0){
        if(AllActions[AllActions.size()-1][0]==1){ //Specific column needs to be plotted.
        if(AllActions.size()==1){   //We want to recover the previous plotted column because it has been modified by certain actions.
                columnPlot = 2; //Default value
        }else{
                columnPlot = AllActions[AllActions.size()-1][2];
            }
            plotall(flag1);
        }else if (AllActions[AllActions.size()-1][0]==4 || AllActions[AllActions.size()-1][0]==5|| AllActions[AllActions.size()-1][0]==6){ //Specific column needs to be plotted.
            columnPlot = AllActions[AllActions.size()-1][3];
            plotall(flag1);
            GetBackupFunc();
        }else if(AllActions[AllActions.size()-1][0]==17){ // If we undo a stacking option.
            VerticalShift=RememberInv[RememberInv.size()-1][1];
            RememberInv.resize(RememberInv.size()-1);
            GetBackupFunc();
            // plotall(flag1);
        }else if (AllActions[AllActions.size()-1][0]==21||AllActions[AllActions.size()-1][0]==7){
            GetBackupFunc();                            //This must come BEFORE columnPlot and plotall cuz we need to recover all deleted columns before setting its previous number.
            columnPlot = AllActions[AllActions.size()-1][2];
            plotall(flag1);
        }else{                              //Same column as before action is plotted.
            GetBackupFunc();
        }

        if(AllActions.size()==1){
            AllActions.clear();
        }else{
            AllActions.resize(AllActions.size()-1);
            RememberInv.resize(AllActions.size()-1); //This is because RememberInv should have the same length as AllActions.
        }
 }
}

void MainWindow::ResetBackup(){ // When new files are opened, we clear all previously stored data.
    AllData.clear();
    AllData.resize(undo);
    AllActions.clear();
}

//*********FORMAT FUNCTIONS**********

void MainWindow::invertOrder(QString datafile, QString outputdatafile){

    // We open the input file
    QByteArray ba = datafile.toLocal8Bit(); //convert fitxer into char c_str2
    const char *c_str2 = ba.data();
    ifstream obre(c_str2, ios::in);
    std::string line;
    QStringList allLines;

    // We prepare the temp file.
    QFileInfo inputputinfo(datafile);
    QString output = inputputinfo.path() + "/Temporal_file.txt";
    QByteArray be = output.toLocal8Bit(); //convert fitxer into char c_str2
    const char *c_str3 = be.data();
    ofstream temp(c_str3, ios::out);

    int p = 0;
    //    Copy all lines into QStringList allLines
    while (std::getline(obre, line)) {                  //Run over rows
            allLines.append(QString::fromStdString(line));
        p++;
    }

    // We paste all lines in temp
    for(int q = 0; q<p; q++){
        if(q==0){                       // In order to prevent that an extra empty line is left at the end of the file.
            temp << allLines.takeLast().toUtf8().constData();
        }else{
            temp << endl << allLines.takeLast().toUtf8().constData();
        }
    }

    obre.close();
    temp.close();

    QFile oldFile (datafile);
    QFile newFile (output);
    QFile outFile (outputdatafile);
    outFile.remove(); // in order to allow a rename later.
    newFile.rename(outputdatafile);
}

bool MainWindow::checkAscendingOrder(QString datafile){ // returns true if the file is in ascending order.
    QVector<QVector<double>> Data;
    int error = getOneSingleData(datafile, Data);

    if(Data[0][0]<Data[0][Data[0].size()-1]){
        return true;
    }else{
        return false;
    }
}

bool MainWindow::descendingOrder(QString datafile, QString outputdatafile, int ignoreLines){ //returns true if order was changed, false if it was already ascending.

    // We open the input file
    QByteArray ba = datafile.toLocal8Bit(); //convert fitxer into char c_str2
    const char *c_str2 = ba.data();
    ifstream obre(c_str2, ios::in);
    std::string line;
    QStringList allLines;

    // First we need to check if datafiles have or not ascending order!!!!!!!!!!!! Return true if ascendingOrder was performed.

    if(!checkAscendingOrder(datafile)){
        //The data already is in descending order.
        // We open the temp output file where good lines are copied.
        QFileInfo inputputinfo(datafile);
        QString output = inputputinfo.path() + "/Temporal_file.txt";
        QByteArray be = output.toLocal8Bit(); //convert fitxer into char c_str2
        const char *c_str3 = be.data();
        ofstream temp(c_str3, ios::out);

        int p = 0;

        //    Copy all lines into QStringList allLines
        while (std::getline(obre, line)) {                  //Run over rows
                if(p==0){                       // In order to prevent that an extra empty line is left at the end of the file.
                    temp << line;
                }else{
                    temp << endl << line;
                }
            p++;
        }

        obre.close();
        temp.close();

        QFile oldFile (datafile);
        QFile newFile (output);
        QFile outFile (outputdatafile);
        outFile.remove(); // in order to allow a rename later.
        newFile.rename(outputdatafile);
        return false;
    }else{
        //The data is in ascending order and needs to be inverted.
        // We open the temp output file where good lines are copied.
        QFileInfo inputputinfo(datafile);
        QString output = inputputinfo.path() + "/Temporal_file.txt";
        QByteArray be = output.toLocal8Bit(); //convert fitxer into char c_str2
        const char *c_str3 = be.data();
        ofstream temp(c_str3, ios::out);

        int p = 0;
        //    Copy all lines into QStringList allLines
        while (std::getline(obre, line)) {                  //Run over rows
            if(p<ignoreLines){                  // We paste the caption
                if(p==0){                       // In order to prevent that an extra empty line is left at the end of the file.
                    temp << line;
                }else{
                    temp << endl << line;
                }
            }else{                              // We remember all lines after the caption.
                allLines.append(QString::fromStdString(line));
            }
            p++;
        }
        //    Paste all lines in temp
        for(int q = 0; q<(p-ignoreLines); q++){
            if(q==0){                       // In order to prevent that an extra empty line is left at the end of the file.
                if(ignoreLines>0){
                    temp << endl;
                }
                temp << allLines.takeLast().toUtf8().constData();
            }else{
                temp << endl << allLines.takeLast().toUtf8().constData();
            }
        }

        obre.close();
        temp.close();
        QMessageBox::information(this, "Information", QString("So far so good.") );

        QFile oldFile (datafile);
        QFile newFile (output);
        QFile outFile (outputdatafile);
        outFile.remove(); // in order to allow a rename later.
        newFile.rename(outputdatafile);
        return true;
    }
}

bool MainWindow::ascendingOrder(QString datafile, QString outputdatafile, int ignoreLines){ //returns true if order was changed, false if it was already ascending.

    // We open the input file
    QByteArray ba = datafile.toLocal8Bit(); //convert fitxer into char c_str2
    const char *c_str2 = ba.data();
    ifstream obre(c_str2, ios::in);
    std::string line;
    QStringList allLines;

    // First we need to check if datafiles have or not ascending order!!!!!!!!!!!! Return true if ascendingOrder was performed.

    if(checkAscendingOrder(datafile)){
        //The data already is in ascending order.
        // We open the temp output file where good lines are copied.
        QFileInfo inputputinfo(datafile);
        QString output = inputputinfo.path() + "/Temporal_file.txt";
        QByteArray be = output.toLocal8Bit(); //convert fitxer into char c_str2
        const char *c_str3 = be.data();
        ofstream temp(c_str3, ios::out);

        int p = 0;

        //    Copy all lines into QStringList allLines
        while (std::getline(obre, line)) {                  //Run over rows
                if(p==0){                       // In order to prevent that an extra empty line is left at the end of the file.
                    temp << line;
                }else{
                    temp << endl << line;
                }
            p++;
        }

        obre.close();
        temp.close();

        QFile oldFile (datafile);
        QFile newFile (output);
        QFile outFile (outputdatafile);
        outFile.remove(); // in order to allow a rename later.
        newFile.rename(outputdatafile);
        return false;
    }else{
        //The data is not in ascending order and needs to be inverted.
        // We open the temp output file where good lines are copied.
        QFileInfo inputputinfo(datafile);
        QString output = inputputinfo.path() + "/Temporal_file.txt";
        QByteArray be = output.toLocal8Bit(); //convert fitxer into char c_str2
        const char *c_str3 = be.data();
        ofstream temp(c_str3, ios::out);

        int p = 0;
        //    Copy all lines into QStringList allLines
        while (std::getline(obre, line)) {                  //Run over rows
            if(p<ignoreLines){                  // We paste the caption
                if(p==0){                       // In order to prevent that an extra empty line is left at the end of the file.
                    temp << line;
                }else{
                    temp << endl << line;
                }
            }else{                              // We remember all lines after the caption.
                allLines.append(QString::fromStdString(line));
            }
            p++;
        }
        //    Paste all lines in temp
        for(int q = 0; q<(p-ignoreLines); q++){
            if(q==0){                       // In order to prevent that an extra empty line is left at the end of the file.
                if(ignoreLines>0){
                    temp << endl;
                }
                temp << allLines.takeLast().toUtf8().constData();
            }else{
                temp << endl << allLines.takeLast().toUtf8().constData();
            }
        }

        obre.close();
        temp.close();
        QMessageBox::information(this, "Information", QString("So far so good.") );

        QFile oldFile (datafile);
        QFile newFile (output);
        QFile outFile (outputdatafile);
        outFile.remove(); // in order to allow a rename later.
        newFile.rename(outputdatafile);
        return true;
    }
}

void MainWindow::deleteFirstLines(QString datafile, QString outputdatafile, int lines){

    // We open the input file
    QByteArray ba = datafile.toLocal8Bit(); //convert fitxer into char c_str2
    const char *c_str2 = ba.data();
    ifstream obre(c_str2, ios::in);
    std::string line;

    // We open the temp output file where good lines are copied.
    QFileInfo inputputinfo(datafile);
    QString output = inputputinfo.path() + "/Temporal_file.txt";
    QByteArray be = output.toLocal8Bit(); //convert fitxer into char c_str2
    const char *c_str3 = be.data();
    ofstream temp(c_str3, ios::out);

    int p = 0;

    while (std::getline(obre, line)) {                  //Run over rows
            if(p>=lines){   // Only copies lines larger than the value introduced by user.
                if(p==0){                       // In order to prevent that an extra empty line is left at the end of the file.
                    temp << line;
                }else{
                    temp << endl << line;
                }
            }
        p++;
    }

    obre.close();
    temp.close();

    QFile oldFile (datafile);
    QFile newFile (output);
    QFile outFile (outputdatafile);
    outFile.remove(); // in order to allow a rename later.
    newFile.rename(outputdatafile);
}

void MainWindow::deleteLastLines(QString datafile, QString outputdatafile, int lines){

    // We open the input file
    QByteArray ba = datafile.toLocal8Bit(); //convert fitxer into char c_str2
    const char *c_str2 = ba.data();
    ifstream obre(c_str2, ios::in);
    std::string line;

    // We open the temp output file where good lines are copied.
    QFileInfo inputputinfo(datafile);
    QString output = inputputinfo.path() + "/Temporal_file.txt";
    QByteArray be = output.toLocal8Bit(); //convert fitxer into char c_str2
    const char *c_str3 = be.data();
    ofstream temp(c_str3, ios::out);

    int p = 0;
    int numOfRows = RowNums(datafile);

    while (std::getline(obre, line)) {                  //Run over rows
            if(p<=(numOfRows-lines)){   // Only copies lines larger than the value introduced by user.
                if(p==0){                       // In order to prevent that an extra empty line is left at the end of the file.
                    temp << line;
                }else{
                    temp << endl << line;
                }
            }
        p++;
    }

    obre.close();
    temp.close();

    QFile oldFile (datafile);
    QFile newFile (output);
    QFile outFile (outputdatafile);
    outFile.remove(); // in order to allow a rename later.
    newFile.rename(outputdatafile);

}

int MainWindow::cleanDatafile(QString datafile, QString outputdatafile, int caption){

        // We open the input file
        QByteArray ba = datafile.toLocal8Bit(); //convert fitxer into char c_str2
        const char *c_str2 = ba.data();
        ifstream obre(c_str2, ios::in);
        std::string line;

        // We open the temp output file where good lines are copied.
        QFileInfo inputputinfo(datafile);
        QString output = inputputinfo.path() + "/Temporal_file.txt";
        QByteArray be = output.toLocal8Bit(); //convert fitxer into char c_str2
        const char *c_str3 = be.data();
        ofstream temp(c_str3, ios::out);

        int respectedRows = 0;
        int p = 1;
        int deletedRows=0;

        while (std::getline(obre, line)) {                  //Run over rows

                if( validate_row(line) || (p<=caption) ){   // Respects validated lines or caption lines.
                    if(p==1){               // In order to prevent that an extra empty line is left at the end of the file.
                        temp << line;
                    }else{
                        temp << endl << line;
                    }
                    respectedRows++;
                }else{                                       // Ignore empty lines or lines with errors.
                    deletedRows++;
                }

            p++;
        }
        obre.close();
        temp.close();

        QFile oldFile (datafile);
        QFile newFile (output);
        QFile outFile (outputdatafile);
        outFile.remove(); // in order to allow a rename later.
        newFile.rename(outputdatafile);

return deletedRows;
}

int MainWindow::estimateRowNums(QString importname, double step){
    double difference, rowNumsDouble;
    QVector<QVector<QVector<QVector<double>>>> TempArr;
    TempArr.resize(1);
    if (getOneData(importname, TempArr)==0){ // If there's no error. BTW. this should create TempArr.
        difference = fabs(TempArr[0][0][0][0] - TempArr[0][0][0][TempArr[0][0][0].size()-1]);
    }
    rowNumsDouble = fabs(difference/step);
    return round(rowNumsDouble);
}

void MainWindow::createNewStep(QString importname, QString exportname, double desiredStep){
    bool equal=false; // checks if an output X value is identical to input X value
    bool found = false;
    int remember,rememberfound;
    double tolerance = 100000; // A value used to check if two double values are equal within double rounding c++ errors.
    QVector<QVector<QVector<QVector<double>>>> TempArr;
    TempArr.resize(1);
    if (getOneData(importname, TempArr)==0){ // If there's no error. BTW. this should create TempArr.
        QVector<QVector<double>> OutputArr;
        OutputArr.resize(TempArr[0][0].size()); //We set proper number of columns
        double currValue = TempArr[0][0][0][0]; //current Value, we will run a for for this value below.
        OutputArr[0].resize(1);
        OutputArr[0][0]=currValue;
        for(int ww=1; ww<TempArr[0][0].size();ww++){ //Here we set the first row of the output, which coincides with the first row of TempArr.
            OutputArr[ww].resize(1);
            OutputArr[ww][0]=TempArr[0][0][ww][0];
        }

            if(TempArr[0][0][0][TempArr[0][0][0].size()-1] > TempArr[0][0][0][0]){ //Numbers of Col 1 increase
                currValue = currValue + desiredStep; //Because we created the first row above.

                while(currValue <= (TempArr[0][0][0][TempArr[0][0][0].size()-1]+desiredStep/2 )){ // We run over rows, creating new rows if possible.
                OutputArr[0].resize(OutputArr[0].size()+1);
                OutputArr[0][OutputArr[0].size()-1] = currValue;
                    for(int q =0; q<TempArr[0][0][0].size()-1;q++){       // We run over rows to search the two values where currVal falls inside in TempArr.
                        //if(currValue == TempArr[0][0][0][q]){           // If currValue is found in TempArr, we will simply copy all row values to OutputArr.
                        if(fabs(currValue - TempArr[0][0][0][q]) < desiredStep/tolerance){
                            equal = true;
                            remember =q;
                            break;
                        //}else if(currValue == TempArr[0][0][0][q+1]){ //Because the for ends at TempArr[0][0][0].size()-2 ! in order to prevent errors below [q+1]
                        }else if(fabs(currValue - TempArr[0][0][0][q+1]) < desiredStep/tolerance){
                            equal = true;
                            remember =q+1;
                            break;
                        }else if((currValue > TempArr[0][0][0][q]) && (currValue < TempArr[0][0][0][q+1])){
                            found=true;
                            rememberfound=q;
                            break;
                        }
                    }

                    for(int w =1; w< TempArr[0][0].size();w++){ // We run over columns
                        OutputArr[w].resize(OutputArr[w].size()+1);
                        if(equal){
                            OutputArr[w][OutputArr[w].size()-1] = TempArr[0][0][w][remember];
                        }else if(found){
                            OutputArr[w][OutputArr[w].size()-1] = TempArr[0][0][w][rememberfound]+(currValue-TempArr[0][0][0][rememberfound])*(TempArr[0][0][w][rememberfound+1]-TempArr[0][0][w][rememberfound])/(TempArr[0][0][0][rememberfound+1]-TempArr[0][0][0][rememberfound]);
                        }
                    }
                    equal = false;
                    found = false;

                currValue = currValue+desiredStep;
                }

            }else if(TempArr[0][0][0][TempArr[0][0][0].size()-1] < TempArr[0][0][0][0]){ // Numbers of Col 1 decrease
                currValue = currValue - fabs(desiredStep); //Because we created the first row above.

                while(currValue >= (TempArr[0][0][0][TempArr[0][0][0].size()-1]-fabs(desiredStep)/2)){ // We run over rows, creating new rows if possible.
                OutputArr[0].resize(OutputArr[0].size()+1);
                OutputArr[0][OutputArr[0].size()-1] = currValue;
                    for(int q =0; q<TempArr[0][0][0].size()-1;q++){       // We run over rows to search the two values where currVal falls inside in TempArr.
            //          if(currValue == TempArr[0][0][0][q]){           // If currValue is found in TempArr, we will simply copy all row values to OutputArr.
                    if(fabs(currValue - TempArr[0][0][0][q]) < desiredStep/tolerance){           // If currValue is found in TempArr, we will simply copy all row values to OutputArr.
                            equal = true;
                            remember =q;
                            break;
       //               }else if(currValue == TempArr[0][0][0][q+1]){ //Because the for ends at TempArr[0][0][0].size()-2 ! in order to prevent errors below [q+1]
                    }else if(fabs(currValue - TempArr[0][0][0][q+1]) < desiredStep/tolerance){ //Because the for ends at TempArr[0][0][0].size()-2 ! in order to prevent errors below [q+1]
                            equal = true;
                            remember =q+1;
                            break;
                        }else if((currValue < TempArr[0][0][0][q]) && (currValue > TempArr[0][0][0][q+1])){
                            found=true;
                            rememberfound=q;
                            break;
                        }
                    }

                    for(int w =1; w< TempArr[0][0].size();w++){ // We run over columns
                        OutputArr[w].resize(OutputArr[w].size()+1);
                        if(equal){
                            OutputArr[w][OutputArr[w].size()-1] = TempArr[0][0][w][remember];
                        }else if(found){
                            OutputArr[w][OutputArr[w].size()-1] = TempArr[0][0][w][rememberfound]+(currValue-TempArr[0][0][0][rememberfound])*(TempArr[0][0][w][rememberfound+1]-TempArr[0][0][w][rememberfound])/(TempArr[0][0][0][rememberfound+1]-TempArr[0][0][0][rememberfound]);
                        }
                    }
                    equal = false;
                    found = false;

                currValue = currValue - fabs(desiredStep);
                }
            }
        //SaveSingleData(importname, OutputArr);
        //QMessageBox::information(this, "Information", QString("Attempting to save in %1 an array of %2 columns and %3 rows") .arg(exportname) .arg(OutputArr.size()) .arg(OutputArr[0].size()));
        SaveSingleData(exportname, OutputArr);
    }
}

double MainWindow::stepNum(QString importname, bool start){
    QVector<QVector<QVector<QVector<double>>>> TempArr;
    TempArr.resize(1);
    if (getOneData(importname, TempArr)==0){ // If there's no error. BTW. this should create TempArr.
        if(start){      // We return the step at the begging of the file
            return fabs(TempArr[0][0][0][1] - TempArr[0][0][0][0]);
        }else{          // We return the step at the end of the file
            return fabs(TempArr[0][0][0][TempArr[0][0][0].size()-2] - TempArr[0][0][0][TempArr[0][0][0].size()-1]);
        }
    }
}

void MainWindow::DeleteTempFolder(QString importnames){
    QFileInfo inputprelputinfo(importnames);
    QString output2 = inputprelputinfo.path() + Tempfolder + "/" + inputprelputinfo.completeBaseName() + "." + inputprelputinfo.suffix();
    QFileInfo outputprelinfo(output2);
    while(QDir(outputprelinfo.path()).exists()){    //IT WAS IF!!!! But I changed it to while to avoid errors created if user is in Temp folder (lag issues creating and destroying folders).
        QString check = outputprelinfo.path(); //We want to check if TempPreview exist, and if it does delete it.
        QDir dir(check);
        dir.removeRecursively();
    }
}

bool MainWindow::checkTempExists(QString importnames){
    QFileInfo inputprelputinfo(importnames);
    QString temp = inputprelputinfo.path() + Tempfolder + "/" + inputprelputinfo.completeBaseName() + "." + inputprelputinfo.suffix();
    QFileInfo inputinfo(temp);
    if(QDir(inputinfo.path()).exists()){ //If the output folder doesnt exist we create it.
        return true;
    }else{
        return false;
    }
}

void MainWindow::CreateTempFolderFiles(QString exportnames){
    QDir dir;
    QFile outputfile(exportnames);
    QFileInfo outputinfo(exportnames);

    //QMessageBox::information(this, "Information", QString("Temp folder will be created if it doesnt exist")  );

    while(!QDir(outputinfo.path()).exists()){ //If the output folder doesnt exist we create it.     //IT WAS IF!!!!
        dir.mkpath(outputinfo.path());
    }
   // QMessageBox::information(this, "Information", QString("Temp folder should now be created in %1") .arg(outputinfo.path()) );
/*
    if (!outputfile.open(QFile::WriteOnly)) {   //If we cannot open the output file we create it.
       QMessageBox::information(this, "Information", QString("Error, we cannot open %1") .arg(outputfile.fileName()) );
    }
*/
    // Next we create the output files.
    if (!outputfile.open(QFile::WriteOnly)) {   //If we cannot open the output file we create it.
        outputfile.open(QFile::WriteOnly);      // With a bit of luck this should create the file.
        inputnames.append(exportnames);         // This will only work for w=0, as desired, see below.
        oldinputnames.append(exportnames);         // This will only work for w=0, as desired, see below.
    }else{
        inputnames.append(exportnames);         // This will only work for w=0, as desired, see below.
        oldinputnames.append(exportnames);         // This will only work for w=0, as desired, see below.
    }
    outputfile.close();                         //Because we opened such file during the previous if.
}

void MainWindow::setRows(QString inputname, QString exportname, int desiredRows){

    QVector<QVector<QVector<QVector<double>>>> TempArr;
    double neededStep;
    TempArr.resize(1);

    // Here we find out the step value that will yield the desired number of rows.

    if (getOneData(inputname, TempArr)==0){ // If there's no error. BTW. this should create TempArr.
        neededStep=fabs(TempArr[0][0][0][0]-TempArr[0][0][0][TempArr[0][0][0].size()-1])/(desiredRows-1);
    }

    // Now we just call createNewStep function.

    createNewStep(inputname, exportname, neededStep);
}

void MainWindow::SaveAFA(bool rewritte){
    QString output;
    QDir dir;
    // To check if Temp folder exists. NOT USED IN THE PRESENT IMPLEMENTAITON, MAYBE USEFUL IN THE FUTURE.
    // bool checkTemp;
    inputnames.clear();
    oldinputnames.clear();
    exportnames.clear();
    replacements.resize(importnames.size());

    // We could check if Temp folder exists. NOT USED IN THE PRESENT IMPLEMENTAITON, MAYBE USEFUL IN THE FUTURE.
    // checkTemp = checkTempExists(importnames[0]);

    // If the Temp folder exist we delete it.
    DeleteTempFolder(importnames[0]);

    for(int q=0; q<importnames.size(); q++){ //Run over files

        QFileInfo info(importnames.at(q));
        if(rewritte){                           // We set the inputnames and exportnames, which are the same if we rewritte.
            exportnames.append(importnames[q]);
            inputnames.append(importnames[q]);
            oldinputnames.append(importnames[q]);
        }else{                                  // Here I should check if file already exist, and if so, use those as inputs.
            // First we set output filename
            exportnames.append(importnames[q]);
            QFileInfo inputputinfo(exportnames[q]);
            output = inputputinfo.path() + Tempfolder + "/" + inputputinfo.completeBaseName() + "." + inputputinfo.suffix();
            exportnames[q]=output;
            output.clear();
            // Next we create the output directory and files, in case it doesnt exist. Also sets inputnames.
            CreateTempFolderFiles(exportnames[q]);
        }

        // QMessageBox::information(this, "Information3", QString("Inputnames is %1") .arg(inputnames[q]) );

        for(int w=0;w<AllFormatActions.size();w++){  // Run over actions.
            if(w==0){
                inputnames[q] = importnames[q];
            }else{
                inputnames[q] = exportnames[q];// We make our operations over the same files. The problem is only to know where the 1st file [0] is in TempPreview or not.
                //QMessageBox::information(this, "Information", QString("Somehow I got in here. AFA size is %1") .arg(AllFormatActions.size()) );
            }
            int y = AllFormatActions[w][0].toInt();
            switch (y) {
                case 1:{
                   // QMessageBox::information(this, "Information", QString("The function is called, input file is: %1\noutput file is %2\nFind string is: %3\nReplace string is: %4") .arg(inputnames[q]) .arg(exportnames[q]) .arg(AllFormatActions[w][1]) .arg(AllFormatActions[w][2]) );
                    replacements[q] = replaceString(inputnames[q],exportnames[q], AllFormatActions[w][1], AllFormatActions[w][2], true);
                break;
                }
                case 2:{
                    // QMessageBox::information(this, "Information", QString("Rewritte (save) is true or false: %1 and we want to change this filename\n\n %2\n\nWhich should be\n\n%3\n\n by this\n\n %4\n\n with this extension: %5. \n(Previous extension was %6)") .arg(rewritte) .arg(importnames[q]) .arg(inputnames[q]) .arg(exportnames[q]) .arg(AllFormatActions[w][1]) .arg(AllFormatActions[w][2]));
                    changeExtension(oldinputnames[q],exportnames[q],importnames[q], AllFormatActions[w][1], AllFormatActions[w][2], rewritte);
                break;
                }
                case 3:{
                    // We use importnames instead of inputnames because this way we ensure no information is lost.
                    createNewStep(importnames[q],exportnames[q], AllFormatActions[w][1].toDouble());
                    // QMessageBox::information(this, "Information", QString("We want to create %1 \nfrom %2\n and with a step of %3") .arg(exportnames[q]) .arg(importnames[q]) .arg(AllFormatActions[w][1].toDouble()) );
                break;
                }
                case 4:{
                    // We use importnames instead of inputnames because this way we ensure no information is lost.
                    setRows(importnames[q], exportnames[q], AllFormatActions[w][1].toInt());
                break;
                }
                case 5:{
                    cleanDatafile(inputnames[q], exportnames[q], AllFormatActions[w][1].toInt());
                break;
                }
                case 6:{
                    deleteFirstLines(inputnames[q], exportnames[q], AllFormatActions[w][1].toInt());
                break;
                }
                case 7:{
                    deleteLastLines(inputnames[q], exportnames[q], AllFormatActions[w][1].toInt());
                break;
                }
                case 8:{
                    ascendingOrder(inputnames[q], exportnames[q], AllFormatActions[w][1].toInt());
                    break;
                }
                case 9:{
                    descendingOrder(inputnames[q], exportnames[q], AllFormatActions[w][1].toInt());
                    break;
                }
                case 10:{
                    invertOrder(inputnames[q], exportnames[q]);
                    break;
                }
            }
        }
        rememberExt.clear();
       // QMessageBox::information(this, "Information", QString("Import are now: \n\n%1") .arg(importnames[0]) );
    }
}

int MainWindow::replaceString(QString nameOfFile, QString nameOfOutputFile, QString find, QString replace, bool writteResult){
    int linesReplaced =0;
    QTextCodec *codec = QTextCodec::codecForLocale();

    // Read file
    QFile file(nameOfFile);
    QFile outputfile(nameOfOutputFile);
    QDir dir;
    QTextStream myText(&file);
    QString text, output;

    if (!file.open(QFile::ReadOnly)) {
        qDebug() << "Error opening for read: " << file.errorString();
    }
    text = myText.readAll();
    file.close();

    //string newtext = text.toUtf8().constData();

    linesReplaced =text.count(find);

    if(writteResult){   //We writte the solution if replaceQ is true. Else this function only finds and counts number of characters.
        text.replace(find, replace);

      //  QMessageBox::information(this, "Information", QString("The error will come soon for %1") .arg(outputfile.fileName()));

        // Write file
        if (!outputfile.open(QFile::WriteOnly)) {
            qDebug() << "Error opening for write: " << outputfile.errorString();

         //   QMessageBox::information(this, "Information", QString("The file %1 cannot be opened now") .arg(outputfile.fileName()));
        }
        outputfile.write(codec->fromUnicode(text));
        outputfile.close();
    }
    return linesReplaced;
}

bool MainWindow::DetectForbidden(QString word){
    if(word.contains(".") || word.contains("|") || word.contains("?") || word.contains("*") || word.contains("\\") || word.contains("\"") || word.contains("/") || word.contains(";") || word.contains(":") || word.contains(">") || word.contains("<") ){
        return 1;
    }else{
        return 0;
    }
}

bool MainWindow::DetectSomeForbidden(QString word){ // Allows dot character.
    if(word.contains("|") || word.contains("?") || word.contains("*") || word.contains("\\") || word.contains("\"") || word.contains("/") || word.contains(";") || word.contains(":") || word.contains(">") || word.contains("<") ){
        return 1;
    }else{
        return 0;
    }
}

void MainWindow::changeExtension(QString inputname, QString exportname, QString &importnames, QString answer1, QString previousExt, bool rewritte){
    QString output, text;
    QTextCodec *codec = QTextCodec::codecForLocale();
    QFile inputfile(inputname);
    QDir dir;
    QTextStream myText(&inputfile);
    QFileInfo infoExp(exportname);
    QFileInfo infoInp(inputname);

    if(infoExp.path() != infoInp.path()){ // If the files are different, then we must change output's name and set its contents.

        if(previousExt.isEmpty()){  // We check if the extension was previously changed.

            //QMessageBox::information(this, "Information", QString("1- File directories are different and extensions was not changed before.") );
            // 1- We rename the output file
            output = infoExp.path() + "/" + infoExp.completeBaseName() + "." + answer1;
            QFile::rename(exportname, output);
            exportname=output;
            output.clear();

            // 2- We copy the text from the input
            if (!inputfile.open(QFile::ReadOnly)) {
                qDebug() << "Error opening for read: " << inputfile.errorString();
            }
            text = myText.readAll();
            inputfile.close();

            // 3- We paste the text in the output
            QFile outputfile(exportname);
            if (!outputfile.open(QFile::WriteOnly)) {
                qDebug() << "Error opening for write: " << outputfile.errorString();
            }
            outputfile.write(codec->fromUnicode(text));
            outputfile.close();

            // 3- I update importnames[q] passed by reference/
            if(rewritte){
                importnames=exportname;
            }
        }else{ // I think the program will never use this option, but i leave it here just in case.

            //QMessageBox::information(this, "Information", QString("2- File directories are different and extensions was changed before. It was: %1") .arg(previousExt) );
            // 1- I delete the outputs mistakenly created by SaveAFA
            QFile fileRemove (exportname);
            fileRemove.remove();

            // 2- I set proper name and location of ouput file which should exist (here it would be nice to check if it exists before proceeding to avoid errors.).
            output = infoExp.path() + "/" + infoExp.completeBaseName() + "." + previousExt;
            exportname=output;

            // 3- I rename the output file with the proper extension
            output = infoExp.path() + "/" + infoExp.completeBaseName() + "." + answer1;
            QFile::rename(exportname, output);
            exportname=output;
            output.clear();

            // 5- I clear rememberExt ( rememberExt.clear() ) after allActions are executed inside SaveAFA.

            // 3- I update importnames[q] passed by reference/
            if(rewritte){
                importnames=exportname;
            }
        }
    }else{
        if(previousExt.isEmpty()){  // We check if the extension was previously changed.

            //QMessageBox::information(this, "Information", QString("3- File directories are equal and extensions was not changed before.") );
            // 1- We just rename the input file and update the importnames.
            output = infoInp.path() + "/" + infoInp.completeBaseName() + "." + answer1;
            QFile::rename(inputname, output);
            inputname=output;
            output.clear();

            // 3- I update importnames[q] passed by reference/
            if(rewritte){
                importnames=inputname;
            }
        }else{

            //QMessageBox::information(this, "Information", QString("4- File directories are equal and extensions was changed before.") );
            // 1- I delete the outputs mistakenly created by SaveAFA
            QFile fileRemove(exportname);
            fileRemove.remove();

            // 2- I set proper name and location of ouput file which should exist (here it would be nice to check if it exists before proceeding to avoid errors.).
            output = infoExp.path() + "/" + infoExp.completeBaseName() + "." + previousExt;
            exportname=output;

            // 3- I rename the output file with the proper extension
            output = infoExp.path() + "/" + infoExp.completeBaseName() + "." + answer1;
            QFile::rename(exportname, output);
            exportname=output;
            output.clear();

            // 5- I update importnames[q] passed by reference/
            if(rewritte){
                importnames=exportname;
            }
            // 6- I clear rememberExt ( rememberExt.clear() ) after allActions are executed inside SaveAFA.
        }
    }
}

// ******************************************************************************************************************
// ***************************************************** ACTIONS ****************************************************
// ******************************************************************************************************************

// *********************************************** File actions ***********************************************

void MainWindow::on_actionExport_data_triggered(){
    SaveAFA(true);
    QFileInfo inputputinfo(importnames[0]);
    QString check = inputputinfo.path() + Tempfolder; //We want to check if TempPreview exist, and if it does delete it.
    QDir dir(check);
    dir.removeRecursively();
    AllFormatActions.clear();
    prevExt.clear();
    importnames.clear(); // This is optional, and might be annoying for the user to import the files each time after export, but I think it will prevent errors, specially with the UNDO option.
}

void MainWindow::on_actionImport_data_triggered(){
    importnames =QFileDialog::getOpenFileNames(
                this,
                tr("Open File"),
                "C://",
                "All files (*.*);;Ascii Files (*.asc)");

    if (importnames.size()>0){
        if(filenames.size()==1){
            ui->statusbar->showMessage("One file has been succesfully imported. Operate with this file in the Format menu.",10000);
        }else{
            ui->statusbar->showMessage("Files have been succesfully imported. Operate with these files in the Format menu.",10000);
        }
        QFileInfo inputputinfo(importnames[0]);
        QString check = inputputinfo.path() + Tempfolder; //We want to check if TempPreview exist, and if it does delete it.
        QDir dir(check);
        dir.removeRecursively();
        AllFormatActions.clear();
        prevExt.clear();
    }else{
        //QMessageBox msgBox;
        //msgBox.setInformativeText("Warning: No valid file could be opened");
        //msgBox.Information(1)
        //msgBox.exec();
        QMessageBox::warning(
            this,
            tr("Warning!"),
            tr("No file could be opened") );
    }
}

void MainWindow::on_actionSave_as_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    QVector<QVector<double>> tempVec;
    QStringList pieces;
    QString result;

    //ACTION takes place here.
    newFileDirectory = QFileDialog::getSaveFileName(this, tr("Set name for the new file"), "C://", "Text file (*.txt);;Ascii File (*.asc)");
    pieces = newFileDirectory.split(".");

    if(filenames.size()==1){
        result = newFileDirectory;
        newFileNames += result; //newFileNames is a QStringList
    }else{
        for(int i =0; i<filenames.size(); i++){             // Run over files.

            for(int q = 0; q<pieces.length()-1;q++){        // Run over all pieces separated by a point, and stop just before the extension.
                result.append(pieces.at(q));
            }
            result.append(QString("_%1.") .arg(i+1));          // Add number of file.
            result.append(pieces.at(pieces.length()-1));    // Add extension

            //getOneSingleData(filenames.at(i), tempVec);
            //SaveSingleData(result,tempVec);
            newFileNames += result; //newFileNames is a QStringList
            result.clear();
        }
    }
    SaveAll(filenames, newFileNames, AllActions);

    for(int i =0; i<filenames.size(); i++){             // Run over files.
        filenames[i]=newFileNames[i];
    }

    newFileNames.clear();
    AllActions.clear();
    VerticalShift=0; //reset some parameters in case they were manipulated.
    ui->statusbar->showMessage("All Files have been saved.",10000);

}

void MainWindow::on_actionExport_column_to_single_file_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }else if(FileColNums[1]<=2){
        QMessageBox::warning(this, "Warning", QString("To perform this operation, selected datafiles should have more than 2 columns.") );
        return;
    }else if(AllActions.size()>0){
        QMessageBox::warning(this, "Warning!", QString("Before proceeding, first save all changes (Ctrl+S)."));
        return;
    }

    int columnExport = QInputDialog::getInt(this, "Input",QString("Current datafiles has %1 columns. \nTo export a single column into a new file introduce column number (from 2 to %1) to be plotted and operated:") .arg(FileColNums[1]),0,2,FileColNums[1]);

    //ACTION takes place here.
    newFileDirectory = QFileDialog::getSaveFileName(this, tr("Set name for the new file"), "C://", "Text file (*.txt);;Ascii File (*.asc)");

    QVector<QVector<double>> tempVec, tempVecLarge;
    int RowNumsAtI;
    tempVec.resize(1); //First column x.
    int error=0;
    for(int i =0; i<filenames.size(); i++){                         // Run over files.
        error += getOneSingleData(filenames.at(i), tempVecLarge);
        tempVec.resize(tempVec.size()+1);
        RowNumsAtI = RowNums(filenames.at(i));                      // To save computational time.
        tempVec[0].resize(RowNumsAtI);
        tempVec[i+1].resize(RowNumsAtI);

        for(int q = 0; q<RowNumsAtI; q++){
            tempVec[0][q]=tempVecLarge[0][q];                       // This is a bit redundant, but making it load just one is a bit to complex.
            tempVec[i+1][q]=tempVecLarge[columnExport-1][q];
        }
    }
    if (error >=1){
        QMessageBox::information(this,"Information", QString("Warning: Empty lines or lines with invalid data have been detected in %1 files. Please make sure that the datafiles content exhibit proper format and numbers.") .arg(error));
    }


    SaveSingleData(newFileDirectory,tempVec); //Can it create new files?
    newFileNames.clear();
    VerticalShift=0; //reset some parameters in case they were manipulated.
    ui->statusbar->showMessage(QString("Column %1 have been saved.") .arg(columnExport),10000);
}

void MainWindow::on_actionNew_triggered(){
    ui->plot->clearGraphs();
    ui->plot->replot();
    //plot();
    ui->statusbar->showMessage("Plots are cleared",10000);
}

void MainWindow::on_actionOpen_triggered(){
    filenames =QFileDialog::getOpenFileNames(
                this,
                tr("Open File"),
                "C://",
                "All files (*.*);;Ascii Files (*.asc)");
    //We plot the first file
    if (filenames.size()>0){
        findColRowNums(filenames); //Now we know if all files have or don't have the same amount of rows and columns, and their ranges. Info stored in FileRowNums[] and FileColNums[].
        ColumnMinSize=FileColNums[1]; //From now on we use ColumnMinSize since some functions will modify the num of columns in AllData[][][][] without modifying filenames.

        flag1=0; //We only want to plot the first graph.
        VerticalShift=0; //reset some parameters in case they were manipulated before opening this new files.
        ResetBackup();
        columnPlot = 2;      //column number to plot by default is 2.
        ReferenceColumn = 2; //reference column number is by default 2.
        getData(filenames, AllData);

        for(int q=0; q<filenames.size(); q++){ // We check that AllData has been loaded properly
            if(ColNums(filenames.at(q))<2){
                QMessageBox::warning(this, tr("Warning!"), tr("Files could not be opened due to the presence of corrupted files.\n(Tip #1: Check that there are no empty datafiles, and their numbers contain dots instead of commas as decimals without thousand separators)\n(Tip #2: In order to modify the format from 'Format' options first import files from 'Import datafile(s)')") );
                return;
            }
        }

        if(log){    //If log scale was previously selected, when opening a new file we go back to linear scale to prevent errors with negative numbers.
            ui->plot->yAxis->setScaleType(QCPAxis::ScaleType::stLinear);
            QSharedPointer<QCPAxisTicker> mLinTicker (new QCPAxisTicker);
            log = false;
            ui->plot->yAxis->setTicker(mLinTicker);
            rescale_rob(flag1);
            ui->plot->replot();
            ui->plot->update();
        }

        plotall(flag1);

        if(filenames.size()==1){
            ui->statusbar->showMessage("Column 2 is ploted.",10000);
        }else{
            ui->statusbar->showMessage("First plot is plotted. In order to plot up to first 10 plots select option Graph->Plot->All data. In order to change the 10 plot limit select option Graph->Plot->Set max. number of plots.",10000);
        }

    }else{
        //QMessageBox msgBox;
        //msgBox.setInformativeText("Warning: No valid file could be opened");
        //msgBox.Information(1)
        //msgBox.exec();
        QMessageBox::warning(
            this,
            tr("Warning!"),
            tr("No file could be opened") );
    }
}

void MainWindow::on_actionSave_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Attention!", "Data will be overwritten, prior backup is advised.\nAre you sure you want to save?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        SaveAll(filenames, filenames, AllActions);
        AllActions.clear();
        VerticalShift=0; //reset some parameters in case they were manipulated.
        ui->statusbar->showMessage("All Files have been saved.",10000);
      //qDebug() << "Yes was clicked";
    }
}

void MainWindow::on_actionSave_in_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    /*else if(AllActions.size()>0){
        QMessageBox::warning(this, "Warning!", QString("Before proceeding, first save all changes (Ctrl+S)."));
        return;
    }*/

    newFileDirectory = QFileDialog::getExistingDirectory(this, tr("Set directory"), "C://");
    QStringList pieces;
    for(int i =0; i<filenames.size(); i++){
        pieces = filenames.at(i).split("/");
        newFileNames += newFileDirectory; //newFileNames is a QStringList
        newFileNames[i].append(QString("%1%2").arg("/").arg(pieces.at( pieces.length()-1)));
    }

    SaveAll(filenames, newFileNames, AllActions);

    for(int i =0; i<filenames.size(); i++){             // Run over files.
        filenames[i]=newFileNames[i];
    }

    newFileNames.clear();
    AllActions.clear();
    VerticalShift=0; //reset some parameters in case they were manipulated.
    ui->statusbar->showMessage("All Files have been saved.",10000);

}

void MainWindow::on_actionMerge_files_triggered(){ //Combines pairs of datafiles (those opened with those selected in reference files)
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    QVector<QVector<double>> AllDataQ, ReferenceDataQ, TemporalArrQ;

    if(filenames.size()==0){ //Bunch of possible error warnings and exiting code with return.
        QMessageBox::information(this,"Info", QString("To combine pairs of datafiles new files need to be opened first, then files to be combined must be selected in 'Analysis'->'Reference data'->'Add data files'. Opened files will be merged with reference files in alphabetical order.") );
        return;
    }else if(referencenames.size()==0){
        QMessageBox::information(this,"Info", QString("Please, select files to be combined in 'Analysis'->'Reference data'->'Add data files'. Current files will be combined in pairs with reference files in alphabetical order.") );
        return;
    }else if(filenames.size()!=referencenames.size()){ //check size of files.
        QMessageBox::information(this,"Warning!", QString("The same number of opened files as reference files must be selected. Opened files will be merged with reference files in alphabetical order.") );
        return;
    }else if(AllActions.size()>0){
        QMessageBox::information(this,"Warning!", QString("Since some actions have been performed, changes must be saved before combining pairs of files.") );
        return;
    }
    bool error = false;                            //    I check if number of rows is the same for all files
    for(int q=0;q<filenames.size();q++){                                                        // run over all files
        if(RowNums(filenames.at(q))!=RowNums(referencenames.at(q))){
            error=true;
        }
    }
    if(error){
        QMessageBox::information(this,"Error", QString("Some files do not share the same number of rows. Merge could not be executed.") );
        return;
    }

    QMessageBox::StandardButton reply;
    QMessageBox::StandardButton reply2;
    reply = QMessageBox::question(this, "Attention!", "Are you sure you want to merge datafiles? Opened files will be overwritten, prior backup is advised.",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
       reply2 = QMessageBox::question(this, "Question", "Do you want to ignore the first column of the Reference files?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply2 == QMessageBox::Yes) {
            int original; //original number of cols
            int error =0;
            for(int q=0;q<filenames.size();q++){                                                        // run over all files
                error += getOneSingleData(filenames.at(q),AllDataQ);
                getOneSingleData(referencenames.at(q),ReferenceDataQ);
                original = AllDataQ.size();
                AllDataQ.resize(original+ReferenceDataQ.size()-1);
                for(int e =1;e<ReferenceDataQ.size();e++){                                          // run obver columns
                    AllDataQ[e+original-1].resize(ReferenceDataQ[e].size());
                    for(int w=0;w<AllDataQ[0].size();w++){                                          // run over all rows
                        AllDataQ[e+original-1][w]=ReferenceDataQ[e][w];
                    }
                }
            TemporalArrQ.resize(AllDataQ.size());
             for(int a=0;a<AllDataQ.size();a++){
                 TemporalArrQ[a].resize(AllDataQ[a].size());
                 for(int s=0; s<AllDataQ[a].size();s++){
                     TemporalArrQ[a][s]=AllDataQ[a][s];
                 }
             }
            SaveSingleData(filenames.at(q), TemporalArrQ);
            TemporalArrQ.clear();
            }
            if(error>=1){
                QMessageBox::information(this,"Information", QString("Warning: Empty lines or lines with invalid data have been detected in %1 files. Please make sure that the datafiles content exhibit proper format and numbers.") .arg(error) );
            }
            ui->statusbar->showMessage(QString("All data has been merged."),10000);
        }else{
            int original; //original number of cols
            for(int q=0;q<AllData[0].size();q++){                                                        // run over all files
                error += getOneSingleData(filenames.at(q),AllDataQ);
                getOneSingleData(referencenames.at(q),ReferenceDataQ);
                original = AllDataQ.size();
                AllDataQ.resize(original+ReferenceDataQ.size());
                for(int e =0;e<ReferenceDataQ.size();e++){                                          // run obver columns
                    AllDataQ[e+original].resize(ReferenceDataQ[e].size());
                    for(int w=0;w<AllDataQ[0].size();w++){                                          // run over all rows
                        AllDataQ[e+original][w]=ReferenceDataQ[e][w];
                    }
                }
                TemporalArrQ.resize(AllDataQ.size());
                 for(int a=0;a<AllDataQ.size();a++){
                     TemporalArrQ[a].resize(AllDataQ[a].size());
                     for(int s=0; s<AllDataQ[a].size();s++){
                         TemporalArrQ[a][s]=AllDataQ[a][s];
                     }
                 }
                SaveSingleData(filenames.at(q), TemporalArrQ);
                TemporalArrQ.clear();
            }
            if(error>=1){
                QMessageBox::information(this,"Information", QString("Warning: Empty lines or lines with invalid data have been detected in %1 files. Please make sure that the datafiles content exhibit proper format and numbers.") .arg(error) );
            }
            ui->statusbar->showMessage(QString("All data has been merged."),10000);
        }
        getData(filenames, AllData);
        plotall(flag1);
    }
}

void MainWindow::on_actionMerge_files_2_triggered(){ //Merge multiple datafiles into a signle file.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    QVector<QVector<QVector<QVector<double>>>> TemporalArr1;
    QVector<QVector<QVector<QVector<double>>>> Output;
    if(filenames.size()<=0){
        QMessageBox::warning(this,"Warning", QString("To merge multiple datafiles into a single file first more than one file needs to be opened.") );
        return;
    }else if(AllActions.size()>0){
        QMessageBox::information(this,"Warning!", QString("Since some actions have been performed, changes must be saved before merging all files into a single file.") );
        return;
    }

    int check = RowNums(filenames.at(0));
   // int check=AllData[0][0][0].size();
    bool error = false;                            //    I check if number of rows is the same for all files. THIS LOOP SHOULD OPEN FILES ONE BY ONE, EXPLORING THEM AND NOT RELYING ON ALLDATA, WHICH WILL HAVE ONLY 10 FILES MAX IN THE FUTURE
    for(int q=0;q<filenames.size();q++){        // run over all files
        if(RowNums(filenames.at(q))!=check){
            error=true;
        }
    }
    if(error){
        QMessageBox::warning(this,"Error", QString("Some files do not share the same number of rows. Files could not be merged.") );
        return;
    }

    Output.resize(1);
    Output[0].resize(1);
    QMessageBox::StandardButton reply2;
    int lol =0;
   reply2 = QMessageBox::question(this, "Question", "Do you want to ignore the first column of the files to be merged?",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply2 == QMessageBox::Yes) {
        lol = getSingleData(filenames.at(0), 0, TemporalArr1);

        //We introduce the first file to the Output, with all its columns.
            Output[0][0].resize(TemporalArr1[0][0].size());

        for(int w=0;w<TemporalArr1[0][0].size();w++){               // We run over columns
            Output[0][0][w].resize(TemporalArr1[0][0][w].size());
            for(int e=0; e<TemporalArr1[0][0][w].size();e++){       // We run over rows
                Output[0][0][w][e]=TemporalArr1[0][0][w][e];
            }
        }
        int rememberCols=TemporalArr1[0][0].size()-1;

        // We introduce the next files to the Output, without the first column.
        for(int q=1;q<filenames.size();q++){                       // We run over files, skipping the first file
          lol += getSingleData(filenames.at(q), q, TemporalArr1);
          for(int w=1;w<TemporalArr1[0][q].size();w++){             // We run over columns, skipping the first column
          Output[0][0].resize(Output[0][0].size()+1);
          rememberCols++;
          Output[0][0][rememberCols].resize(TemporalArr1[0][q][w].size()); //ERROR!!!!!!
              for(int e=0; e<TemporalArr1[0][q][w].size();e++){     // We run over rows
                  Output[0][0][rememberCols][e]=TemporalArr1[0][q][w][e];
              }
          }
        }
    }else{
        lol = getSingleData(filenames.at(0), 0, TemporalArr1);
        Output[0][0].resize(TemporalArr1[0][0].size());
        for(int w=0;w<TemporalArr1[0][0].size();w++){               // We run over columns
            Output[0][0][w].resize(TemporalArr1[0][0][w].size());
            for(int e=0; e<TemporalArr1[0][0][w].size();e++){       // We run over rows
                Output[0][0][w][e]=TemporalArr1[0][0][w][e];
            }
        }
        int rememberCols=TemporalArr1[0][0].size()-1;

        for(int q=1;q<filenames.size();q++){                       // We run over files, skipping the first file
          lol += getSingleData(filenames.at(q), q, TemporalArr1);
          for(int w=0;w<TemporalArr1[0][q].size();w++){             // We run over columns, skipping the first column
          Output[0][0].resize(Output[0][0].size()+1);
          rememberCols++;
          Output[0][0][rememberCols].resize(TemporalArr1[0][q][w].size()); //ERROR!!!!!!
              for(int e=0; e<TemporalArr1[0][q][w].size();e++){     // We run over rows
                  Output[0][0][rememberCols][e]=TemporalArr1[0][q][w][e];
              }
          }
        }

    }
    if(lol>=1){
        QMessageBox::information(this,"Information", QString("Warning: Empty lines or lines with invalid data have been detected in %1 files. Please make sure that the datafiles content exhibit proper format and numbers.") .arg(lol) );
    }
    //Here I need to input a filename.
    QString fileName = QFileDialog::getSaveFileName(this,
        tr("Save"), "",
        tr("ASCII (*.asc);;Text (*.txt);;All Files (*)"));

    SaveData(fileName, Output);
    TemporalArr1.clear();
    Output.clear();

        ui->statusbar->showMessage(QString("All data has been merged into a single file."),10000);
}

void MainWindow::on_actionColumns_to_files_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }else if(FileColNums[1]<=2){
        QMessageBox::warning(this, "Warning", QString("To perform this operation, selected datafiles should have more than 2 columns.") );
        return;
    }else if(AllActions.size()>0){
        QMessageBox::warning(this, "Warning!", QString("Before proceeding, first save all changes (Ctrl+S)."));
        return;
    }

    int columnExport = QInputDialog::getInt(this, "Input",QString("Current datafiles has %1 columns. \nTo export all columns into new files introduce 0. \nTo export a single column into new files introduce column number (from 2 to %1) to be plotted and operated:") .arg(FileColNums[1]),0,0,FileColNums[1]);

    if (columnExport ==1){
        QMessageBox::warning(this, "Warning!", QString("Please, either select 0 to export all columns into new files, or select a column number between 2 and %1") .arg(FileColNums[1]) );
        return;
    }

    //ACTION takes place here.
    newFileDirectory = QFileDialog::getExistingDirectory(this, tr("Set directory to save new files"), "C://");
    QVector<QVector<double>> tempVec, tempVecLarge;
    QStringList pieces,thefilename;
    int RowNumsAtI;
    int error = 0;
    for(int i =0; i<filenames.size(); i++){                             // Run over files.
        pieces = filenames.at(i).split("/");
        thefilename = pieces.at( pieces.length()-1).split(".");         //Name + extension.
        newFileNames += newFileDirectory;
        error+=getOneSingleData(filenames.at(i), tempVecLarge);

        if(columnExport==0){                                            // We need to export a new file for each column.
            for(int w = 1; w<ColNums(filenames.at(i));w++){             // Run over columns
                tempVec.resize(2);
                tempVec[0].resize(tempVecLarge[0].size());
                tempVec[1].resize(tempVecLarge[w].size());
                newFileNames[i].append(QString("%1%2_Col%3%4%5") .arg("/") .arg(thefilename.at( thefilename.length()-2)) .arg(w+1) .arg(".") .arg(thefilename.at( thefilename.length()-1)));
                for(int q = 0; q<tempVecLarge[0].size(); q++){          // Run over rows
                    tempVec[0][q]=tempVecLarge[0][q];
                    tempVec[1][q]=tempVecLarge[w][q];
                }
                SaveSingleData(newFileNames[i],tempVec);                // We save a file for each column
                newFileNames.removeLast();
                newFileNames += newFileDirectory;
            }
            tempVec.clear();
        }else{
            newFileNames[i].append(QString("%1%2_Col%3%4%5") .arg("/") .arg(thefilename.at( thefilename.length()-2)) .arg(columnExport) .arg(".") .arg(thefilename.at( thefilename.length()-1)));
            tempVec.resize(2);
            RowNumsAtI = RowNums(filenames.at(i));                      // To save computational time.
            tempVec[0].resize(RowNumsAtI);
            tempVec[1].resize(RowNumsAtI);
            for(int q = 0; q<RowNumsAtI; q++){
                tempVec[0][q]=tempVecLarge[0][q];
                tempVec[1][q]=tempVecLarge[columnExport-1][q];
            }
            SaveSingleData(newFileNames[i],tempVec); //Can it create new files?
            tempVec.clear();
        }
    }
    if(error>=1){
        QMessageBox::information(this,"Information", QString("Warning: Empty lines or lines with invalid data have been detected in %1 files. Please make sure that the datafiles content exhibit proper format and numbers.") .arg(error) );
    }
    newFileNames.clear();
    VerticalShift=0; //reset some parameters in case they were manipulated.
    ui->statusbar->showMessage("All Files have been saved.",10000);
}

void MainWindow::on_actionExit_triggered(){

    if (importnames.size()>0){ //We want to check if TempPreview exist, and if it does delete it.
        QFileInfo inputputinfo(importnames[0]);
        QString check = inputputinfo.path() + Tempfolder;
        QDir dir(check);
        dir.removeRecursively();
    }

    qApp->quit();
}

// *********************************************** Prepare data actions *******************************************

void MainWindow::on_actionLinear_baseline_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    for(int q=0; q<AllData[0].size();q++){
        LinearBaseline(AllData[0][q], columnPlot);
    }
    plotall(flag1);
    ui->statusbar->showMessage(QString("A linear baseline has been subtracted"),10000);
    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]=10;
    AllActions[AllActions.size()-1][1]=columnPlot;
    VerticalShift=0; //We reset the vertical stacking
}

void MainWindow::on_actionBall_like_baseline_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);
    double result = getMyDouble(QString("Set ball radii. Suggested value = %1. Please note that this action can take large computational time: Avoid using too large ball radii."). arg(abs(AllData[0][0][0][0]-AllData[0][0][0][AllData[0][0][0].size()-1])/2));
    for(int q=0; q<AllData[0].size();q++){
        BallBaseline(AllData[0][q], columnPlot, result);
    }
    plotall(flag1);
    ui->statusbar->showMessage(QString("A ball-like baseline has been subtracted"),10000);
    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]=18;
    AllActions[AllActions.size()-1][1]=columnPlot;
    RememberInv[RememberInv.size()-1][0]=result;
    VerticalShift=0; //We reset the vertical stacking
}

void MainWindow::on_actionTrim_triggered(){ // right now this function will trim ALL columns! Future versions should be able to trim just one column.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    double min,max;
    double AbsMin, AbsMax; //Absolute min and max amongst all spectra
    double minMax, maxMin; //min max is the lowest max value, and max min is the largest lowest value amongst all files.
    AbsMin = FindMinFile(filenames.at(0));
    AbsMax = FindMaxFile(filenames.at(0));
    minMax=AbsMin;
    maxMin=AbsMax;

    for(int q=1; q<filenames.size();q++){
        min =FindMinFile(filenames.at(q));
        max=FindMaxFile(filenames.at(q));
        if(AbsMin>min){
            AbsMin=min;
        }
        if(AbsMax<max){
            AbsMax=max;
        }
        if(minMax<min){
            minMax=min;
        }
        if(maxMin>max){
            maxMin=max;
        }
    }

    double result1,result2;
    bool error = 1;

    while(error){
        result1= getMyDouble(QString("Introduce lowest value to trim the plot (plot starts at %1). Introduced value must be lower than %2.") .arg(AbsMin) .arg(maxMin));
        if(!(result1<maxMin)){
          QMessageBox::warning(this, "Warning!", QString("Introduced value must be lower than %1. Please try again.") .arg(maxMin) );
        }else{
            error =0;
        }
    }

    error=1;
    while(error){
        result2= getMyDouble(QString("Introduce largest value to trim the plot (plot ends at %1). Introduced value must be larger than %2.") .arg(AbsMax) .arg(minMax));
        if(!(result2>minMax)){
          QMessageBox::warning(this, "Warning!", QString("Introduced value must be larger than %1. Please try again.") .arg(minMax) );
        }else{
            error =0;
        }
    }

    for(int p=0; p<AllData[0].size();p++){
        Trim(AllData[0][p], result1, result2);
    }
    plotall(flag1);
    ui->statusbar->showMessage(QString("All plots have been trimed from %1 to %2. This action affected all columns.").arg(result1).arg(result2),10000);
    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(2);

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(1);
    AllActions[AllActions.size()-1][0]=9;
    RememberInv[RememberInv.size()-1][0]=result1;
    RememberInv[RememberInv.size()-1][1]=result2;

}

void MainWindow::on_actionClean_spikes_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);
    int spikes = 0;
    int recsigma=0; //recommended sigma and points.
    double sigmas;
    int wpow2=0;
    int points = QInputDialog::getInt(this, "Input", QString("Tolerance parameters: Introduce number of statistical points (recommended %1, minimum 5)") .arg(AllData[0][0][0].size()/20),0,-2147483647,2147483647,1);

        for(int w=1;w<10;w++){ //Trying to find a recommended value for sigma.
            wpow2=w*w*w;
            spikes = spikinator(AllData[0][0], points, columnPlot, wpow2, 0);
            if((spikes>0) && (100*spikes/AllData[0][0][columnPlot-1].size()<10.0)){ //If some spikes were detected but represent less than 15% of the data.
                recsigma=wpow2;
                //QMessageBox::information(this, "Information", QString("at sigma %1, %2 spikes should be corrected") .arg(wpow2) .arg(spikes));
            }
        }

    if(recsigma==0){
        sigmas = getMyDouble("Tolerance parameters: Introduce number sigma tolerance (recommended 8)");
    }else{
        sigmas = getMyDouble(QString("Tolerance parameters: Introduce number sigma tolerance (recommended %1 or lower)") .arg(recsigma));
    }
   spikes = 0;
    for (int q=0; q<AllData[0].size();q++){             //run over files
        spikes = spikes + spikinator(AllData[0][q], points, columnPlot, sigmas, 1);
    }
    plotall(flag1);
    ui->statusbar->showMessage(QString("%1 spikes have been corrected per file") .arg(spikes/AllData[0].size()),10000);
    AllActions.resize(AllActions.size()+1);
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]=16;
    AllActions[AllActions.size()-1][1]=points;
    AllActions[AllActions.size()-1][2]=columnPlot;
    RememberInv[RememberInv.size()-1][0]=sigmas;
}

void MainWindow::on_actionTo_one_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(AllData[0].size());

    double num, inv;
    for (int q=0; q<AllData[0].size();q++){             //run over files
        num = Integrate(AllData[0][q], columnPlot);
        inv = 1/num;
        RememberInv[RememberInv.size()-1][q]=inv;
        MultConsty(AllData[0][q], inv, columnPlot);
    }

    plotall(flag1);
    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]=11;
    AllActions[AllActions.size()-1][1]=columnPlot;
    ui->statusbar->showMessage(QString("All plots have been normalized to one"),10000);
}

void MainWindow::on_actionTo_first_file_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(AllData[0].size());
    double num, inv, num1st;
    num1st= Integrate(AllData[0][0], columnPlot);
    for (int q=0; q<AllData[0].size();q++){
        num = Integrate(AllData[0][q], columnPlot);
        inv = num1st/num;
        RememberInv[RememberInv.size()-1][q]=inv;
        MultConsty(AllData[0][q], inv, columnPlot);
    }
    plotall(flag1);
    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]=13;
    AllActions[AllActions.size()-1][1]=columnPlot;
    ui->statusbar->showMessage(QString("All plots have been normalized. Area of first file was %1") .arg(num1st),10000);
}

void MainWindow::on_actionTo_one_2_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(AllData[0].size());
    double num, inv;
    for (int q=0; q<AllData[0].size();q++){
        num = Maxy(AllData[0][q], columnPlot);
        inv = 1/num;
        RememberInv[RememberInv.size()-1][q]=inv;
        MultConsty(AllData[0][q], inv, columnPlot);
    }
    plotall(flag1);
    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]=12;
    AllActions[AllActions.size()-1][1]=columnPlot;
    ui->statusbar->showMessage(QString("All plots have been normalized to one"),10000);
}

void MainWindow::on_actionTo_first_file_2_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(AllData[0].size());
    double num, inv, num1st;
    num1st= Maxy(AllData[0][0], columnPlot);
    for (int q=0; q<AllData[0].size();q++){
        num = Maxy(AllData[0][q], columnPlot);
        inv = num1st/num;
        RememberInv[RememberInv.size()-1][q]=inv;
        MultConsty(AllData[0][q], inv, columnPlot);
    }
    plotall(flag1);
    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]=14;
    AllActions[AllActions.size()-1][1]=columnPlot;
    ui->statusbar->showMessage(QString("All plots have been normalized to one."),10000);
}

// *********************************************** Files and columns *******************************************

void MainWindow::on_actionSet_Plot_Column_triggered(){          // Set plot column
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    previouscolPlot = columnPlot;

    if (plotNum>0){
    // ACTION takes place here
   // findMinCol(AllData[0]);
    columnPlot = QInputDialog::getInt(this, "Input",QString("Current datafiles has %1 columns. Introduce column number (from 1 to %1) to be plotted and operated:") .arg(ColumnMinSize),0,1,ColumnMinSize);
        if((columnPlot<1) || (columnPlot>ColumnMinSize)){  //This particular if is really unnecesary since the QInputDialog will only allow to introduce valid numbers.
             QMessageBox::warning(this,"Warning!", QString("Column number introduced, %1, is not valid (please, introduce a number between 1 and %2)") .arg(columnPlot) .arg(ColumnMinSize));
             columnPlot=previouscolPlot;  //set back to previous value.
             return;
        }
        if(log){ // If log scale is selected.
            for (int i = 0; i< plotNum;i++){
            double* min_y = std::min_element(AllData[0][i][columnPlot-1].begin(), AllData[0][i][columnPlot-1].end());
                if(*min_y<=0){
                    QMessageBox::warning(this, "Warning!", QString("Some datafiles contain negative y values. Log scale cannot be represented. Select linear scale before attempting to select column number %1.") .arg(columnPlot) );
                    columnPlot= previouscolPlot;
                    return;
                }
            }
        }
    }else{
    QMessageBox::information(
        this,
        tr("Warning"),
        tr("No valid data was opened") );
    }

    plotall(flag1);

    AllActions.resize(AllActions.size()+1);
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]=1;
    AllActions[AllActions.size()-1][1]=columnPlot;
    AllActions[AllActions.size()-1][2]=previouscolPlot;

//    ui->statusbar->showMessage(QString("Column %1 has been selected and plotted").arg(columnPlot),10000);
}

void MainWindow::on_actionSet_x_column_triggered(){             // Rearange columns
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();

    int result1 = QInputDialog::getInt(this, "Input",QString("Introduce first column number to be swapped from 1 to %1") .arg(ColumnMinSize),1,1,ColumnMinSize);
    int result2 = QInputDialog::getInt(this, "Input",QString("Introduce second column number to be swapped from 1 to %1") .arg(ColumnMinSize),1,1,ColumnMinSize);

    for(int q=0; q<AllData[0].size(); q++){
        SwapCols(AllData[0][q], result1, result2);
    }
    plotall(flag1);
    ui->statusbar->showMessage(QString("Columns %1 and %2 have been swapped").arg(result1) .arg(result2),10000);

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]=8;
    AllActions[AllActions.size()-1][1]=result1;
    AllActions[AllActions.size()-1][2]=result2;
}

void MainWindow::on_actionDelete_column_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    if(ColumnMinSize>2){               // If there are more than 2 columns in all files.
        Backup();// It is very important to include the backup BEFORE any data manipulation.

        int result = QInputDialog::getInt(this, "Input",QString("Set column number to delete from 2 to %1") .arg(ColumnMinSize), 0, 2, ColumnMinSize, 1);

        previouscolPlot = columnPlot;
        if(columnPlot>result){ //The number of cols will be reduced by 1.
            columnPlot--;
        }
        if(columnPlot==result){ //The number of cols will be reduced by 1.
            columnPlot=2;
        }

        //ACTION takes place here
        for(int q=0; q<AllData[0].size();q++){  //Run over files.
            DeleteCol(AllData[0][q], result);
        }
        ColumnMinSize--;

        plotall(flag1);
        ui->statusbar->showMessage(QString("Column %1 has been deleted").arg(result),10000);

        AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
        AllActions[AllActions.size()-1].resize(3);
        AllActions[AllActions.size()-1][0]=7;
        AllActions[AllActions.size()-1][1]=result;
        AllActions[AllActions.size()-1][2]=previouscolPlot;
    }else{
        QMessageBox::warning(this, "Warning!", QString("No columns can be deleted. Actual file(s) don't have enough columns. Some files only have %1 columns.") .arg(FileColNums[1]));
    }
}

void MainWindow::on_actionDuplicate_Column_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    Backup();// It is very important to include the backup BEFORE any data manipulation.

    int result = QInputDialog::getInt(this, "Input",QString("Set column number to duplicate from 1 to %1") .arg(ColumnMinSize), 0, 1, ColumnMinSize, 1);

    //ACTION takes place here
    for(int q=0; q<AllData[0].size();q++){      // Run over files.
        DuplicateCol(AllData[0][q], result);    // Duplicates a column to a new column at the end.
    }
    ColumnMinSize++;

    //We update the number of columns. How does delete does it without calling this function?
 //   findColRowNums(filenames); //Now we know if all files have or don't have the same amount of rows and columns, and their ranges. Info stored in FileRowNums[] and FileColNums[].

 //   QMessageBox::information(this, "Information", QString("Now there are %1 columns, FileColNums[1] is %2") .arg(AllData[0][0].size()).arg(FileColNums[1]) );

    if(FileColNums[0]==1){
        QMessageBox::warning(this, "Warning!", QString("Files have a different number of columns. The duplicated column will be added as a new column at the end of each datafile, hence its position won't be the same for all files.") );
    }

    plotall(flag1);
    ui->statusbar->showMessage(QString("Column %1 has duplicated and included as a new column at the end of each file.").arg(result),10000);

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]=22;
    AllActions[AllActions.size()-1][1]=result;
}

void MainWindow::on_actionAdd_reference_column_triggered(){     // Set reference column
    ReferenceColumn = QInputDialog::getInt(this, "Input",QString("Introduce column number (from 1 to %1) to be used for operations with nominal data:") .arg(ColumnMinSize),0,1,ColumnMinSize);
    if((ReferenceColumn<1) || (ReferenceColumn>ColumnMinSize)){ //This if is not really needed since the QInputDialog does not allow to take wrong numbers.
         QMessageBox::warning(this,"Warning!", QString("Column number introduced, %1, is not valid (please, introduce a number between 1 and %2)") .arg(ReferenceColumn) .arg(ColumnMinSize));
         ReferenceColumn=2; //set back to default value.
         return;
     }
        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(2);
        AllActions[AllActions.size()-1][0]=3;
        AllActions[AllActions.size()-1][1]=ReferenceColumn;
}

void MainWindow::on_actionAdd_reference_triggered(){ //Add columns
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }else if(FileColNums[0]==1){
        QMessageBox::warning(this, "Warning", QString("Operation cannot be performed since datafiles contain a different number of columns.") );
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Question:", QString("Do you confirm that Col. #%1 will be added to Col. #%2?") .arg(columnPlot) .arg(ReferenceColumn),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {

        Backup();// It is very important to include the backup BEFORE any data manipulation.
        int previouscolumnPlot = columnPlot;

        //ACTION takes place here.
        for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
            AddColumns(AllData[0][q],columnPlot,ReferenceColumn);
        }
        ColumnMinSize++;
        columnPlot = AllData[0][0].size();
        plotall(flag1);

        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(4);
        AllActions[AllActions.size()-1][0]=19;
        AllActions[AllActions.size()-1][1]=previouscolumnPlot;
        AllActions[AllActions.size()-1][2]=ReferenceColumn;
        AllActions[AllActions.size()-1][3]=previouscolumnPlot;
        ui->statusbar->showMessage(QString("Columns have been added. A new column %1 has been generated.") .arg(columnPlot),10000);
    }
}

void MainWindow::on_actionSubtract_2_triggered(){ // Subtract columns
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }else if(FileColNums[0]==1){
        QMessageBox::warning(this, "Warning", QString("Operation cannot be performed since datafiles contain a different number of columns.") );
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Question:", QString("Do you confirm that Col. #%1 will be subtracted to Col. #%2?") .arg(columnPlot) .arg(ReferenceColumn),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {

        Backup();// It is very important to include the backup BEFORE any data manipulation.
        int previouscolumnPlot = columnPlot;

        //ACTION takes place here
        for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
            SubtractColumns(AllData[0][q],columnPlot,ReferenceColumn);
        }
        ColumnMinSize++;
        columnPlot = AllData[0][0].size();
        plotall(flag1);

        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(3);
        AllActions[AllActions.size()-1][0]=4;
        AllActions[AllActions.size()-1][1]=previouscolumnPlot;
        AllActions[AllActions.size()-1][2]=ReferenceColumn;
        ui->statusbar->showMessage(QString("Columns have been subtracted. A new column %1 has been generated.") .arg(columnPlot),10000);
    }
}

void MainWindow::on_actionDivide_2_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }else if(FileColNums[0]==1){
        QMessageBox::warning(this, "Warning", QString("Operation cannot be performed since datafiles contain a different number of columns.") );
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Question:", QString("Do you confirm that Col. #%1 will be divided by Col. #%2?") .arg(columnPlot) .arg(ReferenceColumn),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        Backup();// It is very important to include the backup BEFORE any data manipulation.
        int previouscolumnPlot = columnPlot;

        //ACTION takes place here
        for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
            DivideColumns(AllData[0][q],columnPlot,ReferenceColumn);
            //QMessageBox::information(this, "Information for debug", QString("Running over files"));
        }
        ColumnMinSize++;

        columnPlot = AllData[0][0].size();
        plotall(flag1);

        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(4);
        AllActions[AllActions.size()-1][0]=5;
        AllActions[AllActions.size()-1][1]=previouscolumnPlot;
        AllActions[AllActions.size()-1][2]=ReferenceColumn;
        AllActions[AllActions.size()-1][3]=previouscolumnPlot;
        ui->statusbar->showMessage(QString("Columns have been divided. A new column %1 has been generated.") .arg(columnPlot),10000);
    }
}

void MainWindow::on_actionMultiplu_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }else if(FileColNums[0]==1){
        QMessageBox::warning(this, "Warning", QString("Operation cannot be performed since datafiles contain a different number of columns.") );
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Question:", QString("Do you confirm that Col. #%1 will be multiplied to Col. #%2?") .arg(columnPlot) .arg(ReferenceColumn),
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        Backup();// It is very important to include the backup BEFORE any data manipulation.
        int previouscolumnPlot = columnPlot;

        // ACTION takes place here
        for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
            MultiplyColumns(AllData[0][q],columnPlot,ReferenceColumn);
            //QMessageBox::information(this, "Information for debug", QString("Running over files"));
        }
        ColumnMinSize++;
        columnPlot = AllData[0][0].size();
        plotall(flag1);

        //QMessageBox::information(this, "Information for debug", QString("Column %1 is plotted, previous column was %2") .arg(columnPlot) .arg(previouscolumnPlot));

        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(4);
        AllActions[AllActions.size()-1][0]=6;
        AllActions[AllActions.size()-1][1]=previouscolumnPlot;
        AllActions[AllActions.size()-1][2]=ReferenceColumn;
        AllActions[AllActions.size()-1][3]=previouscolumnPlot;
        ui->statusbar->showMessage(QString("Columns have been multiplied. A new column %1 has been generated.") .arg(columnPlot),10000);
    }
}

void MainWindow::on_actionAdd_reference_files_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    referencenames =QFileDialog::getOpenFileNames(
                this,
                tr("Open File"),
                "C://",
                "All files (*.*);;Ascii Files (*.asc)");
    if (referencenames.size()>0){
        ui->statusbar->showMessage(QString("%1 reference file(s) have been imported").arg(referencenames.size()),10000);
    }else{
        QMessageBox::information(
            this,
            tr("Warning"),
            tr("No file could be opened") );
    }
}

// *********************************************** Analysis actions ***********************************************

void MainWindow::on_actionClean_interferences_by_FFT_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    //suggested value corresponds to frequencies around 10% of the datarange.
    double suggestion = 10/fabs(AllData[0][0][0][0]-AllData[0][0][0][AllData[0][0][0].size()-1]);
    double lowfreq, highfreq;
    bool error1=1;
    while(error1){
        lowfreq  = getMyDouble(QString("Introduce lowest frequency to filter (an example value is %1).\nHint: To estimate the frequency of the interference to delete subtract the maximum of two consecutive peaks and take its inverse 1/(x_max(i)-x_max(i+1)).") .arg(0.9*suggestion));
        highfreq  = getMyDouble(QString("Introduce maximum frequency to filter (an example value is %1)") .arg(1.1*suggestion));
        if(lowfreq<highfreq){
            error1=0;
        }else{
            QMessageBox::warning(this, "Warning!", "Introduced lowest frequency must be lower than introduced maximum frequency. Please, try again.");
        }
    }

    for(int p=0; p<AllData[0].size();p++){
        Filter(AllData[0][p], columnPlot, lowfreq, highfreq);
    }

    plotall(flag1);
    ui->statusbar->showMessage(QString("Oscillations have been filtered from frequencies %1 to %2. The algorithm used is bare Fourier Transform. Faster FFT method will be implemented in future versions.").arg(lowfreq).arg(highfreq),10000);
    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(2);

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]=20;
    AllActions[AllActions.size()-1][1]=columnPlot;
    RememberInv[RememberInv.size()-1][0]=lowfreq;
    RememberInv[RememberInv.size()-1][1]=highfreq;
}

void MainWindow::on_actionCalculate_modulus_from_KKA_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    int previouscolumnPlot = columnPlot; //This is unnecesary since I decided to overwritte the column.

    for(int p=0; p<AllData[0].size();p++){
        KKA(AllData[0][p], columnPlot);
    }

//I commented this because I dont want to output the results in a new column. I think it is better to overwritte in order to prevent errors when working with multiple files with different number of columns. The user should copy the original column if he doesnt want to lose it. Filter option also works like this.
//    columnPlot = AllData[0][0].size();
    plotall(flag1);
    ui->statusbar->showMessage(QString("The modulus has been calculated by means of Kramers–Kronig analysis."),10000);

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]=21;
    AllActions[AllActions.size()-1][1]=columnPlot;
    AllActions[AllActions.size()-1][2]=previouscolumnPlot;
}

void MainWindow::on_actionAdd_constant_3_triggered(){       // It adds a constant to y axis.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();// It is very important to include the backup BEFORE any data manipulation.
    double result = getMyDouble("Set constant value to be added:");
    //QMessageBox::information(this, "title", QString::number(result));

    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        AddConstant(AllData[0][q],columnPlot,result);
    }

    plotall(flag1);
    ui->statusbar->showMessage(QString("Constant %1 has been added to all plots").arg(result),10000);

    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]=2;
    AllActions[AllActions.size()-1][1]=columnPlot;
    RememberInv[RememberInv.size()-1][0]=result;
    ui->statusbar->showMessage(QString("Constant %1 has been added to column #%2.") .arg(result) .arg(columnPlot),10000);

    //QMessageBox::information(this,"Info", QString("AllActions has a size of %1 times %2. A constant of %3 is added to column %4") .arg(AllActions.size()) .arg(AllActions[AllActions.size()-1].size()) .arg(AllActions[AllActions.size()-1][1]) .arg(columnPlot));
}

void MainWindow::on_actionMultiply_constant_4_triggered(){  // It multiplies a constant to y axis.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);
    double value = getMyDouble("Introduce multiplication value");
    for (int q=0; q<AllData[0].size();q++){
        MultConsty(AllData[0][q], value, columnPlot);
    }
    plotall(flag1);
    ui->statusbar->showMessage(QString("All plots have been multiplied by %1") .arg(value),10000);
    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]=15;
    AllActions[AllActions.size()-1][1]=columnPlot;
    RememberInv[RememberInv.size()-1][0]=value;
}

void MainWindow::on_actionModify_units_2_triggered(){

}

// *********************************************** Graph actions ***********************************************

void MainWindow::on_actionAll_data_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    flag1=1;
    plotall(1);
    ui->statusbar->showMessage("All data is plotted. Remember that the number of plotted data can be changed in 'Graph->Plot->Set max. number of plots'.",10000);
}

void MainWindow::on_actionOnly_first_datafile_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    flag1=0;
    ui->plot->clearGraphs();
    ui->plot->addGraph();
    plotall(flag1);
    ui->statusbar->showMessage("Only first datafile is plotted",10000);
}

void MainWindow::on_actionSet_max_number_of_plots_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    if(AllActions.size()>0){
        QMessageBox::warning(this, "Warning!", QString("Before proceeding, first save all changes (Ctrl+S)."));
        return;
    }
    plotNumLim = QInputDialog::getInt(this, "Input",QString("Introduce limit of files to be plotted (introduce 0 to plot all files). Current limit is %1. Current number of opened files is %2.") .arg(plotNumLim) .arg(filenames.size()),0,0,filenames.size()); //Only takes positive numbers.
    VerticalShift=0; //reset some parameters in case they were manipulated before opening this new files.
    ResetBackup();
    getData(filenames, AllData);
    plotall(flag1);
}

void MainWindow::on_actionRescale_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    rescale_rob(flag1);
    ui->plot->replot();
    ui->plot->update();
    ui->statusbar->showMessage("Graph rescaled. To zoom in use mouse scroll. Individual axis can be zoomed when mouse is on top.",10000);
}

void MainWindow::on_actionUndo_triggered(){
    if(filenames.isEmpty() && importnames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open or import new files to proceed with this action.") );
        return;
    }
    if(AllActions.size()>0){
        GetBackup();
        plotall(flag1);
        ui->statusbar->showMessage(QString("Last action has been undone. A maximum of %1 actions can be undone.") .arg(undo),10000);
    }else if (AllFormatActions.size()>0){
        if(AllFormatActions[AllFormatActions.size()-1][0]=="2"){ //This needs to be done for rename extension option.
          prevExt.resize(prevExt.size()-1);
        }

        if(AllFormatActions.size()==1){
             AllFormatActions.clear();
             QFileInfo inputputinfo(importnames[0]);
             QString check = inputputinfo.path() + Tempfolder; //We want to check if TempPreview exist, and if it does delete it.
             QDir dir(check);
             dir.removeRecursively();
             ui->statusbar->showMessage(QString("Last format action has been undone."),10000);
        }else{
            AllFormatActions.resize(AllFormatActions.size()-1);
            SaveAFA(false); //We only set true for Export, when we want to overwritte the data.
            ui->statusbar->showMessage(QString("Last format action has been undone."),10000);
        }
    }else{
        ui->statusbar->showMessage(QString("No action has been undone. A maximum of %1 actions can be undone.") .arg(undo),10000);
    }

}

void MainWindow::on_actionShow_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    ui->plot->legend->setVisible(true);
    ui->plot->replot();
}

void MainWindow::on_actionHide_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    ui->plot->legend->setVisible(false);
    ui->plot->replot();
}

void MainWindow::on_actionActivated_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    if(AllData[0].size()>1){
        double maxVal = AllData[0][0][columnPlot-1][0];
        double previousVerticalshift = VerticalShift;
        for(int q=0; q<AllData[0].size();q++){
            for(int w=0; w<AllData[0][q][columnPlot-1].size(); w++){
                if(maxVal<AllData[0][q][columnPlot-1][w]){
                    maxVal=AllData[0][q][columnPlot-1][w];
                }
            }
        }

        VerticalShift = getMyDouble(QString("Set stacking value to vertically shift the plots. Suggested value is %1") .arg(maxVal));
        RememberInv.resize(AllActions.size()+1);
        RememberInv[RememberInv.size()-1].resize(2);

       for(int e = 0; e<AllData[0].size();e++){
            VerticalStack(AllData[0][e], e,VerticalShift-previousVerticalshift);
        }

        plotall(flag1);
        ui->statusbar->showMessage(QString("Plots have been shifted vertically with a stacking constant of %1") .arg(VerticalShift),10000);
        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(1);
        AllActions[AllActions.size()-1][0]=17;
        RememberInv[RememberInv.size()-1][0]=VerticalShift;
        RememberInv[RememberInv.size()-1][1]=previousVerticalshift;
    }else{
        QMessageBox::information(this, "Information", QString("Multiple graphs must be plotted to perform the stacking action."));
    }
}

void MainWindow::on_actionDisactivated_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    if(VerticalShift==0){
        QMessageBox::information(this, "Information", QString("Plots are not vertically stacked."));
    }else if(AllData[0].size()==0){
        QMessageBox::information(this, "Information", QString("Multiple graphs must be plotted to perform this action."));
    }else{
        double result = -1*VerticalShift;
        for(int e = 0; e<AllData[0].size();e++){
             VerticalStack(AllData[0][e], e, result);
         }
        VerticalShift=0;
        plotall(flag1);
        ui->statusbar->showMessage(QString("Vertical stack has been undone by a value of %1.") .arg(result),10000);
        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(1);
        AllActions[AllActions.size()-1][0]=18;
        RememberInv[RememberInv.size()-1][0]=result;
    }
}

void MainWindow::on_actionlogarithmic_triggered(){

    for (int i = 0; i< plotNum;i++){
    double* min_y = std::min_element(AllData[0][i][columnPlot-1].begin(), AllData[0][i][columnPlot-1].end());
        if(*min_y<=0){
            QMessageBox::warning(this, "Warning!", QString("Some datafiles contain negative y values. Log scale cannot be represented.") );
            return;
        }
    }

    ui->plot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    log = true;
    ui->plot->yAxis->setTicker(logTicker);
    rescale_rob(flag1);
    ui->plot->replot();
    ui->plot->update();
    ui->statusbar->showMessage(QString("Vertical axis scale is now logarithmic.") ,10000);
}

void MainWindow::on_actionLinear_triggered(){
    ui->plot->yAxis->setScaleType(QCPAxis::ScaleType::stLinear);
    QSharedPointer<QCPAxisTicker> mLinTicker (new QCPAxisTicker);
    log = false;
    ui->plot->yAxis->setTicker(mLinTicker);
    rescale_rob(flag1);
    ui->plot->replot();
    ui->plot->update();
    ui->statusbar->showMessage(QString("Vertical axis scale is now linear.") ,10000);
}

// *********************************************** FORMAT actions ***********************************************

void MainWindow::on_actionInvert_all_order_triggered(){
    if(importnames.isEmpty()){ // We check if some files are imported.
        QMessageBox::warning(this, "Warning", QString("Please, first import new files to proceed with this action.") );
        return;
    }

    AllFormatActions.resize(AllFormatActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllFormatActions[AllFormatActions.size()-1].resize(1);
    AllFormatActions[AllFormatActions.size()-1][0]=QString("10");

    SaveAFA(false); //We only set true for Export, when we want to overwritte the data.

    ui->statusbar->showMessage(QString("All data rows have been sorted in inverse order.  Select Export (Ctrl+E) to save changes. A preview can be found in folder /TempPreview."),10000);
}

void MainWindow::on_actionDescending_triggered(){
    if(importnames.isEmpty()){ // We check if some files are imported.
        QMessageBox::warning(this, "Warning", QString("Please, first import new files to proceed with this action.") );
        return;
    }

        // First we check if there is data that needs to be rearranged into descending order.
    bool ascending = false;
    for(int q = 0; q<importnames.size();q++){
        if(checkAscendingOrder(importnames[q])){
            ascending = true;  // Some data is in ascending order.
        }
    }

    if(!ascending){
        QMessageBox::information(this, "Information", QString("All datafile(s) are already in descending order. No action was performed.") );
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Attention!", "Do you want to rearrange rows in descending order?\n(First column is used as reference)",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {

        int ignoreLines = 0;
        QString message = QString("Do you want to ignore caption lines? \nIf so, introduce the number of top lines that shall not be moved to the bottom of the file.)" );
        ignoreLines = QInputDialog::getInt(this, "Input", message,0,0,2147483647);

        AllFormatActions.resize(AllFormatActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
        AllFormatActions[AllFormatActions.size()-1].resize(2);
        AllFormatActions[AllFormatActions.size()-1][0]=QString("9");
        AllFormatActions[AllFormatActions.size()-1][1]=QString("%1") .arg(ignoreLines);

        SaveAFA(false); //We only set true for Export, when we want to overwritte the data.

        ui->statusbar->showMessage(QString("Data rows have been sorted in descending order.  Select Export (Ctrl+E) to save changes. A preview can be found in folder /TempPreview."),10000);
    }else{
        return;
    }
}

void MainWindow::on_actionAscending_triggered(){
    if(importnames.isEmpty()){ // We check if some files are imported.
        QMessageBox::warning(this, "Warning", QString("Please, first import new files to proceed with this action.") );
        return;
    }

        // First we check if there is data that needs to be rearranged into ascending order.
    bool descending = false;
    for(int q = 0; q<importnames.size();q++){
        if(!checkAscendingOrder(importnames[q])){
            descending = true;  // Some data is in descending order.
        }
    }

    if(!descending){
        QMessageBox::information(this, "Information", QString("All datafile(s) are already in ascending order. No action was performed.") );
        return;
    }


    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Attention!", "Do you want to rearrange rows in ascending order?\n(First column is used as reference)",
                                  QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {

        int ignoreLines = 0;
        QString message = QString("Do you want to ignore caption lines? \nIf so, introduce the number of top lines that shall not be moved to the bottom of the file.)" );
        ignoreLines = QInputDialog::getInt(this, "Input", message,0,0,2147483647);

        AllFormatActions.resize(AllFormatActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
        AllFormatActions[AllFormatActions.size()-1].resize(2);
        AllFormatActions[AllFormatActions.size()-1][0]=QString("8");
        AllFormatActions[AllFormatActions.size()-1][1]=QString("%1") .arg(ignoreLines);

        SaveAFA(false); //We only set true for Export, when we want to overwritte the data.

        ui->statusbar->showMessage(QString("Data rows have been sorted in ascending order.  Select Export (Ctrl+E) to save changes. A preview can be found in folder /TempPreview."),10000);
    }else{
        return;
    }
}

void MainWindow::on_actionDelete_last_lines_triggered(){
    if(importnames.isEmpty()){ // We check if some files are imported.
        QMessageBox::warning(this, "Warning", QString("Please, first import new files to proceed with this action.") );
        return;
    }
    int deletedLines = 0;

    QString message = QString("Introduce the number of rows to be deleted at the end of each datafile.\n(Introduce 0 to leave files unaltered.)" );
    deletedLines = QInputDialog::getInt(this, "Input", message,0,0,2147483647);

    AllFormatActions.resize(AllFormatActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllFormatActions[AllFormatActions.size()-1].resize(2);
    AllFormatActions[AllFormatActions.size()-1][0]=QString("7");
    AllFormatActions[AllFormatActions.size()-1][1]=QString("%1").arg(deletedLines);

    if(deletedLines>0){
        SaveAFA(false); //We only set true for Export, when we want to overwritte the data.
        ui->statusbar->showMessage(QString("%1 lines have been deleted in %2 files.  Select Export (Ctrl+E) to save changes. A preview can be found in folder /TempPreview.") .arg(deletedLines) .arg(importnames.size()),10000);
    }else{
        ui->statusbar->showMessage(QString("No action was performed since selected number of rows to be deleted is 0."), 10000);
    }
}

void MainWindow::on_actionDelete_first_lines_triggered(){
    if(importnames.isEmpty()){ // We check if some files are imported.
        QMessageBox::warning(this, "Warning", QString("Please, first import new files to proceed with this action.") );
        return;
    }
    int deletedLines = 0;

    QString message = QString("Introduce the number of rows to be deleted at the begging of each datafile.\n(Introduce 0 to leave files unaltered.)" );
    deletedLines = QInputDialog::getInt(this, "Input", message,0,0,2147483647);

    AllFormatActions.resize(AllFormatActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllFormatActions[AllFormatActions.size()-1].resize(2);
    AllFormatActions[AllFormatActions.size()-1][0]=QString("6");
    AllFormatActions[AllFormatActions.size()-1][1]=QString("%1").arg(deletedLines);

    if(deletedLines>0){
        SaveAFA(false); //We only set true for Export, when we want to overwritte the data.
        ui->statusbar->showMessage(QString("%1 lines have been deleted in %2 files.  Select Export (Ctrl+E) to save changes. A preview can be found in folder /TempPreview.") .arg(deletedLines) .arg(importnames.size()),10000);
    }else{
        ui->statusbar->showMessage(QString("No action was performed since selected number of rows to be deleted is 0."), 10000);
    }
}

void MainWindow::on_actionDelete_lines_without_data_triggered(){
    if(importnames.isEmpty()){ // We check if some files are imported.
        QMessageBox::warning(this, "Warning", QString("Please, first import new files to proceed with this action.") );
        return;
    }
    int correctedLines = 0;
    int captionLines =0;

    QString message = QString("Introduce the number of caption rows to be respected.\n(Introduce 0 to delete all non-numerical data rows.)" );
    captionLines = QInputDialog::getInt(this, "Input", message,0,0,2147483647);

    AllFormatActions.resize(AllFormatActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllFormatActions[AllFormatActions.size()-1].resize(2);
    AllFormatActions[AllFormatActions.size()-1][0]=QString("5");
    AllFormatActions[AllFormatActions.size()-1][1]=QString("%1").arg(captionLines);

    SaveAFA(false); //We only set true for Export, when we want to overwritte the data.

 ui->statusbar->showMessage(QString("%1 lines have been deleted in %2 files.  Select Export (Ctrl+E) to save changes. A preview can be found in folder /TempPreview.") .arg(correctedLines) .arg(importnames.size()),10000);
}

void MainWindow::on_actionSet_number_triggered(){
    if(importnames.isEmpty()){ // We check if some files are imported.
        QMessageBox::warning(this, "Warning", QString("Please, first import new files to proceed with this action.") );
        return;
    }

    // Here we check that we can read the files.
    QVector<QVector<QVector<QVector<double>>>> TempArr;
    for (int q =0; q<importnames.size(); q++){ // We check if we can read the files.
        TempArr.resize(1);
        if(getOneData(importnames[q], TempArr)==1){
            QMessageBox::warning(this,"Warning", QString("Warning: Empty lines or lines with invalid data have been detected in some files. Please, import files with readable format.") );
            return;
        }
        TempArr.resize(0);
    }

    int desiredRows; // To be introduced by the user.
    bool ok; //If the user does not hit cancell.

        // First we check if all files have the same amount of rows (equalRows)
        int minRows = RowNums(importnames[0]);
        int maxRows = minRows;
        int cache; //to save computational time.
        for(int w=0; w<importnames.size(); w++){
            cache = RowNums(importnames[w]);
            if(cache>maxRows){
                maxRows = cache;
            }else if (cache<minRows){
                minRows = cache;
            }
        }

        if(minRows == maxRows){
           QString message = QString("Imported file(s) have %1 rows of data. Introduce the amount of desired rows: \n(Warning! Introducing smaller number of rows will result in loss of information)") .arg(RowNums(importnames[0])+1);
           desiredRows = QInputDialog::getInt(this, "Input", message,0,2,2147483647,1,&ok);
       }else{
           QString message = QString("Imported files have different number of rows, from %1 to %2. Introduce the amount of desired rows: \n(Warning! Introducing smaller number of rows will result in loss of information)") .arg(minRows+1) .arg(maxRows+1);
           desiredRows = QInputDialog::getInt(this, "Input", message,0,2,2147483647,1,&ok);
           //desiredRows = getMyDouble(QString("Imported files have different number of rows, from %1 to %2. Introduce the amount of desired rows: \n(Warning! Introducing smaller number of rows will result in loss of information)") .arg(minRows) .arg(maxRows) );
        }

        if(!ok){
            return; //We exit the action if no desired row was introduced (cancell) in QInputDialog::getInt.
        }

    AllFormatActions.resize(AllFormatActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllFormatActions[AllFormatActions.size()-1].resize(2);
    AllFormatActions[AllFormatActions.size()-1][0]=QString("4");
    AllFormatActions[AllFormatActions.size()-1][1]=QString("%1").arg(desiredRows);

    SaveAFA(false); //We only set true for Export, when we want to overwritte the data.

    ui->statusbar->showMessage(QString("%1 datafile(s) have been modified. Select Export (Ctrl+E) to save changes. A preview can be found in folder /TempPreview.") .arg(importnames.size()),10000);
}

void MainWindow::on_actionSet_step_triggered(){
    if(importnames.isEmpty()){ // We check if some files are imported.
        QMessageBox::warning(this, "Warning", QString("Please, first import new files to proceed with this action.") );
        return;
    }
    QVector<QVector<QVector<QVector<double>>>> TempArr;
    double remember;
    bool start = true; // To check if all files start with the same number at x.
        TempArr.resize(1);
        if(getOneData(importnames[0], TempArr)==1){
            QMessageBox::warning(this,"Warning", QString("Warning: Empty lines or lines with invalid data have been detected in some files. Please, import files with readable format.") );
            return;
        }
        remember = TempArr[0][0][0][0];
        TempArr.resize(0);

    if(importnames.size()>1){
        for (int q =1; q<importnames.size(); q++){ // We check if we can read the files.
            TempArr.resize(1);
            if(getOneData(importnames[q], TempArr)==1){
                QMessageBox::warning(this,"Warning", QString("Warning: Empty lines or lines with invalid data have been detected in some files. Please, import files with readable format.") );
                return;
            }
            if (remember !=TempArr[0][0][0][0]){
                start = false;
            }
            TempArr.resize(0);
        }
    }

    // First we check what is the step in the files, and wether it is constant or not. Then we ask what is the desired step.
    double stepnum= stepNum(importnames[0], true);
    double desiredStep;
    bool constant=true;         // We assume all files have the same step number from begging to end. We check if it is true below.
    bool equalFiles = true;     // We assume all files are equal in terms of step. We check if it is true below.
    bool rowNums = true;        // We assume that the number of rows coincides with the starting and ending points plus detected step value. If it does not coincide and equalFiles is true, performing the step action is highly recommended to correct missing rows.

    if (stepNum(importnames[0], true) != stepNum(importnames[0], false)){ //IF the step at the beggining of the file is different than at the end, then the step is not constant.
        constant = false;
        rowNums = false;
    }
    if (RowNums(importnames[0]) != estimateRowNums(importnames[0], stepNum(importnames[0], true)) ) {
        rowNums = false;
    }
    //QMessageBox::information(this, "Information", QString("The number of rows is %1 and estimated rows is %2") .arg(RowNums(importnames[0])) .arg(estimateRowNums(importnames[0], stepNum(importnames[0], true))) );

    if(importnames.size()>1){
        for (int q =1; q<importnames.size(); q++){
            if(stepnum!=stepNum(importnames[q], true)){
                equalFiles = false;
            }
            stepnum = stepNum(importnames[q], true);
            if (stepNum(importnames[q], true) != stepNum(importnames[q], false)){ //IF the step at the beggining of the file is different than at the end, then the step is not constant.
                constant=false;
                rowNums = false;
            }
            if (RowNums(importnames[q]) != estimateRowNums(importnames[q], stepNum(importnames[q], true)) ) {
                rowNums = false;
            }
        }
    }

    QString message1, message2, message3, message4;
    if (constant == false){
        message1 = "Some datafile(s) do not have a constant step. ";
    }
    if (equalFiles == false){
        message2 = "Some datafile(s) do not share the same step. ";
    }
    if (start == false){
        message3= "Some datafile(s) start at different x values. ";
    }
    if ((rowNums == false) && (constant == true) && (equalFiles == true)){
        message4= QString("Some datafile(s) exhibit a different number of rows then their expected value from the detected step. \nHence, setting the step to %1 is highly recommended in order to restore missing rows in the datafile(s).") .arg(stepNum(importnames[0], true));
    }

    if(constant && equalFiles && start && rowNums){
        desiredStep = getMyDouble(QString("The current step of the X variable (1st column) is: %1. \nIntroduce new step:\n(Warning! This action will modify the number of rows of the imported files. If a lower step number is introduced the information of the datafile(s) will be permanently decreased)") .arg(stepnum) );
    }else if((constant && !equalFiles) || (constant && !start) || (constant && !rowNums) ){
        desiredStep = getMyDouble(QString("Some warning(s) have been detected: %1%2%3%4 \nThe detected step of the X variable (1st column) is: %5. \nIntroduce new step:\n(Warning! This action will modify the number of rows of the imported files. If a lower step number is introduced the information of the datafile(s) will be permanently decreased).") .arg(message1) .arg(message2) .arg(message3) .arg(message4) .arg(stepnum) );
    }else{
        int reply = QMessageBox::question(this, "Attention!", "It has been detected that some datafile(s) do not exhibit a constant step of the X variable (1st column). Performing this action is not advised. Are you sure you want to proceed with this action?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
        desiredStep = getMyDouble(QString("Some warning(s) have been detected: %1%2%3 \nThe current step of the X variable (1st column) is around: %4. \nSince the step value of X is not constant, performing this action is not advised.\nIntroduce new step:\n(Warning! This action will modify the number of rows of the imported files. If a lower step number is introduced the information of the datafile(s) will be permanently decreased).") .arg(message1) .arg(message2) .arg(message3) .arg(stepnum) );
    }
    if(cancell){         // If cancell is pressed after getMyDouble.
        cancell = false; // We return it to the original false value.
        return;          // We exit this function doing nothing.
    }

    if(desiredStep == 0){
        QMessageBox::warning(this, "Warning!", QString("The introduced step value must be non zero. A suggested value should be around %1") .arg(stepnum) );
        return;
    }

    // Here I should call the function of step number (to be created), with desiredStep, then implement AFA. The funcion will modify all columns and change the number of rows (either increase or decrease it if desiredStep is higher or lower than intional step).

    AllFormatActions.resize(AllFormatActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllFormatActions[AllFormatActions.size()-1].resize(2);
    AllFormatActions[AllFormatActions.size()-1][0]=QString("3");
    AllFormatActions[AllFormatActions.size()-1][1]=QString("%1").arg(desiredStep);

    SaveAFA(false); //We only set true for Export, when we want to overwritte the data.

    ui->statusbar->showMessage(QString("%1 datafile(s) have been modified. Select Export (Ctrl+E) to save changes. A preview can be found in folder /TempPreview") .arg(importnames.size()),10000);

    //This was used before AFA was implemented.
//    for (int q =0; q<importnames.size(); q++){
//        createNewStep(importnames[q], desiredStep); //This function overwrittes. Should be called from AFA instead.
//    }
}

void MainWindow::on_actionReplace_characters_triggered(){
    if(importnames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first import new files to proceed with this action.") );
        return;
    }

    QString answer1, answer2, output;
    answer1 = QInputDialog::getText(this, "Input", "Introduce the character or text string to be searched and replaced.\n(Tip: 'tab' and 'enter' characters can be also pasted below, but caution is advised.)");
    if(answer1.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Text string is empty. No operation was performed.") );
        return;
    }
    answer2 = QInputDialog::getText(this, "Input", "Introduce the character or text string to be used as replacement.\n(Tip #1: 'tab' and 'enter' characters can be also pasted below, but caution is advised.)\n(Tip #2: Note that dialog below can be left empty to erase the selected string from the datafile.)");

    AllFormatActions.resize(AllFormatActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllFormatActions[AllFormatActions.size()-1].resize(3);
    AllFormatActions[AllFormatActions.size()-1][0]=QString("1");
    AllFormatActions[AllFormatActions.size()-1][1]=answer1;
    AllFormatActions[AllFormatActions.size()-1][2]=answer2;

    SaveAFA(false); //We only set true for Export, when we want to overwritte the data.

    int suma=0; //For information purposes only (see below)
    for(int q = 0; q<importnames.size(); q++){
        suma=suma+replacements[q];
    }

    ui->statusbar->showMessage(QString("The string %1 has been replaced by the string %2 a total of %3 times in %4 datafiles. Select Export (Ctrl+E) to save changes. A preview can be found in folder /TempPreview") .arg(answer1) .arg(answer2) .arg(suma) .arg(importnames.size()),10000);
}

void MainWindow::on_actionExtensions_triggered(){
    if(importnames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first import new files to proceed with this action.") );
        return;
    }

    QString answer1, answer2;
    QStringList pieces, nameext;

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Question", "Do you want to rename the extensions? If so, changes can be previsualized in the folder /TempPreview and apply them by clicking 'Export format' (Ctrl+E)", QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        answer1 = QInputDialog::getText(this, "Input", "Introduce the name of the extension without dot (e.g. introduce 'txt' or 'asc').");
        if(answer1.isEmpty()){
            QMessageBox::warning(this, "Warning", QString("File names cannot be empty. No operation was performed.") );
            return;
        }else if(DetectSomeForbidden(answer1)){
            QMessageBox::warning(this, "Warning", QString("Some illegal characters were detected. \nThe illegal characters are: \\ \" , | ? * / ; : > and < \nNo operation was performed.") );
            return;
        }

        AllFormatActions.resize(AllFormatActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
        AllFormatActions[AllFormatActions.size()-1].resize(3);
        AllFormatActions[AllFormatActions.size()-1][0]=QString("2");
        AllFormatActions[AllFormatActions.size()-1][1]=answer1;
        if(prevExt.size()>=1){
        AllFormatActions[AllFormatActions.size()-1][2]=prevExt[prevExt.size()-1]; //Could give an error if prevExt is empty.
        }

        SaveAFA(false); //We only set true for Export, when we want to overwritte the data.
        prevExt.resize(prevExt.size()+1);   //I also decrease it by one in the UNDO OPTION!
        prevExt[prevExt.size()-1]=answer1;

        /*
        for(int q = 0; q<importnames.size(); q++){
            changeExtension(importnames[q], answer1);
        }*/

        ui->statusbar->showMessage(QString("The extension of imported files have been modified. Results can be previewed in folder %1. To save changes click 'File->Export format' (Ctrl+E).") .arg(Tempfolder),10000);
    }else{
      ui->statusbar->showMessage(QString("No action was performed"),10000);
      return;
    }
}

void MainWindow::on_actionFiles_triggered(){
    if(importnames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first import new files to proceed with this action.") );
        return;
    }

    QMessageBox w;
    w.setText("Select one of the following options.");
    w.setInformativeText("1- Set new file names \n2- Replace a text string in the filenames \n3- Append a text string at the start of the filename\n4- Append a text string at the end of the filename \n(Warning: Changes will be immediately applied to imported data, hence prior backup is recommended.)");
    w.setWindowIcon(QPixmap(Logo));

    //THESE STRINGS ARE INDEED USED. DO NOT CHANGE THIS PIECE OF CODE!
    QPushButton* New = w.addButton("New file names", QMessageBox::YesRole);
    QPushButton* Replace = w.addButton("Replace a string", QMessageBox::YesRole);
    QPushButton* AppendStart = w.addButton("Append at start", QMessageBox::YesRole);
    QPushButton* AppendEnd = w.addButton("Append at end", QMessageBox::YesRole);
    QPushButton* Cancel = w.addButton("Cancel", QMessageBox::YesRole);
    w.setDefaultButton(Cancel);
    int ret = w.exec();
    QString answer1, answer2, output;
    QStringList pieces, nameext;


    switch(ret){
        case 0:{    // New file names

            answer1 = QInputDialog::getText(this, "Input", "Introduce the name of the files");
            if(answer1.isEmpty()){
                QMessageBox::warning(this, "Warning", QString("File names cannot be empty. No operation was performed.") );
                return;
            }else if(DetectForbidden(answer1)){
                QMessageBox::warning(this, "Warning", QString("Some illegal characters were detected. \nThe illegal characters are: \\ \" . , | ? * / ; : > and < \nNo operation was performed.") );
                return;
            }

            int reply = QMessageBox::question(this, "Attention!", "Data will be overwritten, prior backup is advised.\nAre you sure you want to save?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::No) {
                return;
            }

            QStringList allLines; // Here we will writte all names.
            allLines.append(QString("New file name:\t\tOriginal file name:\n"));

            if(importnames.size()==1){

                QFileInfo info(importnames.at(0));
                output = info.path() + "/" + answer1 + "." + info.suffix();
                //output = info.path() + "/" + info.completeBaseName() + "." + info.completeSuffix();

                allLines.append(answer1 + "." + info.suffix() + "\t\t" + info.completeBaseName() + "." + info.completeSuffix());

                QFile::rename(importnames.at(0), output);
                importnames[0]=output;
                output.clear();                

            }else if(importnames.size()>1){
                for(int q=0; q<importnames.size();q++){

                QFileInfo info(importnames.at(q));
                output = info.path() + "/" + answer1 + QString("_%1.") .arg(q+1) + info.suffix();

                allLines.append(answer1 + QString("_%1.") .arg(q+1) + info.suffix() + "\t\t" + info.completeBaseName() + "." + info.completeSuffix());

                QFile::rename(importnames.at(q), output);
                importnames[q]=output;
                output.clear();
                }
            }

            // We prepare the output log file.
            QFileInfo inputputinfo(importnames.at(0));
            QString logFileName = "/DataPro_log/FileNames_log.txt";
            QString output = inputputinfo.path() + logFileName;
            QFileInfo logFile(output);
            QDir dir;

            dir.mkpath(logFile.path());

            QByteArray be = output.toLocal8Bit(); //convert fitxer into char c_str2
            const char *c_str3 = be.data();
            ofstream temp(c_str3, ios::out);

            // We paste all lines in FileNames_log.txt
            for(int q = 0; q<allLines.length(); q++){
                    temp << allLines.at(q).toUtf8().constData() << endl;
            }
            temp.close();

            ui->statusbar->showMessage(QString("%1 file(s) have been renamed. Changes can be tracked in the log file: '%2'.") .arg(importnames.size()) .arg(logFileName),10000);
        }
        break;

        case 1:{    // Replace a string
            answer1 = QInputDialog::getText(this, "Input", "First introduce the string to be replaced");
            if(answer1.isEmpty()){
                QMessageBox::warning(this, "Warning", QString("Can't find empty strings. No action has been performed."));
                return;
            }
            answer2 = QInputDialog::getText(this, "Input", "Introduce new string");
            if(DetectSomeForbidden(answer2)){
                            QMessageBox::warning(this, "Warning", QString("Some illegal characters were detected. \nThe illegal characters are: \\ \" , | ? * / ; : > and < \nNo operation was performed.") );
                            return;
            }

            for(int q = 0; q<importnames.size();q++){       // Checking that we are not going to empty any filename. Filenames cannot be empty!
                QFileInfo info(importnames.at(q));
                QString check = info.completeBaseName();
                QString testing= info.completeBaseName();
                if(testing.replace(answer1, answer2).isEmpty()){
                    QMessageBox::warning(this, "Warning", QString("File names cannot be empty. No action has been performed.") );
                    return;
                }
            }

            bool findanswer1 = false;

            for(int q = 0; q<importnames.size();q++){
                QFileInfo info(importnames.at(q));
                QString check = info.completeBaseName();

                if(check.contains(answer1)){
                    findanswer1 =true;
                }

                check.replace(answer1, answer2);

                output = info.path() + "/" + check + "." + info.suffix();
                QFile::rename(importnames.at(q), output);
                importnames[q]=output;
                output.clear();
                check.clear();
            }

            if(!findanswer1){
                QMessageBox::warning(this, "Warning", QString("String %1 was not found in the filenames. No action has been performed.") .arg(answer1));
            }

        }
        break;

        case 2:{    // Append at start
            answer1 = QInputDialog::getText(this, "Input", "Introduce string to be appended at the start of each file name.");
            if(answer1.isEmpty()){
                QMessageBox::warning(this, "Warning", QString("Can't find empty strings. No action has been performed."));
                return;
            }else if(DetectSomeForbidden(answer1)){
                QMessageBox::warning(this, "Warning", QString("Some illegal characters were detected. \nThe illegal characters are: \\ \" , | ? * / ; : > and < \nNo operation was performed.") );
                return;
            }

            for(int q = 0; q<importnames.size();q++){
                QFileInfo info(importnames.at(q));
                output = info.path() + "/" + answer1 + info.completeBaseName() + "." + info.suffix();
                QFile::rename(importnames.at(q), output);
                importnames[q]=output;
                output.clear();
            }
        return;
        }
        break;

        case 3:{    // Append at end
            answer1 = QInputDialog::getText(this, "Input", "Introduce string to be appended at the end of each file name.");
            if(answer1.isEmpty()){
                QMessageBox::warning(this, "Warning", QString("Can't find empty strings. No action has been performed."));
                return;
            }else if(DetectSomeForbidden(answer1)){
                QMessageBox::warning(this, "Warning", QString("Some illegal characters were detected. \nThe illegal characters are: \\ \" , | ? * / ; : > and < \nNo operation was performed.") );
                return;
            }

            for(int q = 0; q<importnames.size();q++){
                QFileInfo info(importnames.at(q));
                output = info.path() + "/" + info.completeBaseName() + answer1 + "." + info.suffix();
                QFile::rename(importnames.at(q), output);
                importnames[q]=output;
                output.clear();
            }
        return;
        }
        case 4:{    // Cancell

        return;
        }

    }
}

// *********************************************** About actions ***********************************************

void MainWindow::on_actionInformation_triggered(){
    QMessageBox msgBox;
    msgBox.setText("<center>DataPro v. 0.2.7</center>\n<center>Robert Oliva Vidal</center>\n<center>2020</center>\n<center>This is a free version, use at your own risk.\n</center><center>You can send suggestions to the whatsapp number +34634119994</center>");
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowIcon(QPixmap(Logo));
    msgBox.exec();
}

// ******************************************************************************************************************
// ***************************************************** EVENTS *****************************************************
// ******************************************************************************************************************

void MainWindow::clickedGraph(QMouseEvent *event){
    QPoint point = event->pos();
    double x = ui->plot->xAxis->pixelToCoord(point.x());
    double y = ui->plot->yAxis->pixelToCoord(point.y());
    ui->statusbar->showMessage(QString("x = %1, y = %2. Values copied to clipboard (press Ctrl+V to paste elsewhere).") .arg(x) .arg(y) ,10000);
    string result =std::to_string(x) + "\t" + std::to_string(y);
    toClipboard(result);
}

/*
void MainWindow::keyPressed(QKeyEvent *event2){ // I was unsuccesful to detect key pressed.
    if(event2->key() == Qt::Key_0){
        QMessageBox::information(this, "Information", QString("EUREKA"));
    }
}*/

void MainWindow::doubleclickedGraph(QMouseEvent *event){
    QPoint point = event->pos();
    double x = ui->plot->xAxis->pixelToCoord(point.x());
    double y = ui->plot->yAxis->pixelToCoord(point.y());
    ui->statusbar->showMessage(QString("x = %1, y = %2. Values copied to clipboard (press Ctrl+V to paste elsewhere). Double click to set tracer.") .arg(x) .arg(y) ,10000);
    string result =std::to_string(x) + "\t" + std::to_string(y);
    toClipboard(result);

    QCPItemTracer *tracer = new QCPItemTracer(ui->plot);

    tracer->position->setCoords(x,y);
 //   tracer->position->coords();
    tracer->updatePosition();
    ui->plot->replot();
    ui->plot->update();
    ui->plot->clearItems();
}

void MainWindow::toClipboard(const std::string &s){
    OpenClipboard(0);
    EmptyClipboard();
    HGLOBAL hg=GlobalAlloc(GMEM_MOVEABLE,s.size());
    if (!hg){
        CloseClipboard();
        return;
    }
    memcpy(GlobalLock(hg),s.c_str(),s.size());
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT,hg);
    CloseClipboard();
    GlobalFree(hg);
}


//****************************************************** END ******************************************************

// USeful Notes for the programmer:

//qDebug() << "Yes was clicked";
//QMessageBox::information(this, "Information", QString("Here I output number %1 and %2") .arg(i) .arg(j));
//ui->statusbar->showMessage(QString("%1 reference file(s) have been imported").arg(referencenames.size()),10000);
//VerticalShift = getMyDouble(QString("Set stacking value to vertically shift the plots. Suggested value is %1") .arg(maxVal));


