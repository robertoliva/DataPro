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

//*********FILE FUNCTIONS***************

void MainWindow::RunOverActions(QVector<QVector<double>> &TemporalArr, int q, int &TempCol, int &TempColRef){
    for(int w=0;w<AllActions.size();w++){                  // Run over all actions
        int y = (int) AllActions[w][0].toInt();            // Maybe the (int) was redundant since i think at the beggining actions was a double.
        switch (y) {
            case 1:{
                TempCol = AllActions[w][1].toInt();
            break;
            }
            case 2:{
                AddConstant(TemporalArr,AllActions[w][1].toInt(),AllActions[w][2].toDouble());
            break;
            }
            case 3:{
                TempColRef = AllActions[w][1].toInt();
            break;
            }
            case 4:{
                SubtractColumns(TemporalArr, AllActions[w][1].toInt(), AllActions[w][2].toInt());
            break;
            }
            case 5:{
                DivideColumns(TemporalArr, AllActions[w][1].toInt(), AllActions[w][2].toInt());
            break;
            }
            case 6:{
                MultiplyColumns(TemporalArr, AllActions[w][1].toInt(), AllActions[w][2].toInt());
            break;
            }
            case 7:{
                DeleteCol(TemporalArr, AllActions[w][1].toInt());
            break;
            }
            case 8:{
                SwapCols(TemporalArr, AllActions[w][1].toInt(), AllActions[w][2].toInt());
            break;
            }
            case 9:{
                 Trim(TemporalArr, RememberInv[w][0], RememberInv[w][1]);
            break;
            }
            case 10:{
                 LinearBaseline(TemporalArr, AllActions[w][1].toInt());
            break;
            }
            case 11:{
                MultConsty1(TemporalArr, AllActions[w][1].toInt());
            break;
            }
            case 12:{
                MultConsty2(TemporalArr, AllActions[w][1].toInt());
            break;
            }
            case 13:{
                MultConsty(TemporalArr, RememberInv[w][q], AllActions[w][1].toInt());
            break;
            }
            case 14:{
                MultConsty(TemporalArr, RememberInv[w][q], AllActions[w][1].toInt());
            break;
            }
            case 15:{
                MultConsty3(TemporalArr, AllActions[w][2].toDouble(), AllActions[w][1].toInt());
            break;
            }
            case 16:{
                spikinator(TemporalArr, AllActions[w][1].toInt(), AllActions[w][2].toInt(), RememberInv[w][0], 1);
            break;
            }
            case 17:{
                VerticalStack(TemporalArr, q, RememberInv[w][0]);
            break;
            }
            case 18:{
                BallBaseline(TemporalArr, AllActions[w][1].toInt(), RememberInv[w][0]);
            break;
            }
            case 19:{
                AddColumns(TemporalArr, AllActions[w][1].toInt(), AllActions[w][2].toInt());
            break;
            }
            case 20:{
                Filter(TemporalArr, AllActions[w][1].toInt(), RememberInv[w][0], RememberInv[w][1]);
            break;
            }
            case 21:{
                KKA(TemporalArr, AllActions[w][2].toInt());
            break;
            }
            case 22:{
                DuplicateCol(TemporalArr, AllActions[w][2].toInt());
            break;
            }
            case 23:{
                LaplacianSmooth(TemporalArr, AllActions[w][1].toInt(), AllActions[w][2].toInt());
            break;
            }
            case 24:{
                differentiate(TemporalArr, AllActions[w][1].toInt());
            break;
            }
            case 25:{
                integrate(TemporalArr, AllActions[w][1].toInt());
            break;
            }
            case 26:{
                AddConstantX(TemporalArr,RememberInv[w][0]);
            break;
            }
            case 27:{
                ExponentiateX(TemporalArr,RememberInv[w][0]);
            break;
            }
            case 28:{
                logarithm(TemporalArr,AllActions[w][1].toInt(),RememberInv[w][0]);
            break;
            }
            case 29:{
                logarithmX(TemporalArr,RememberInv[w][0]);
            break;
            }
            case 30:{
                Exponentiate(TemporalArr,AllActions[w][1].toInt(),RememberInv[w][0]);
            break;
            }
            case 31:{
                UnitsEnergy(TemporalArr,AllActions[w][1].toInt(),AllActions[w][2].toInt());
            break;
            }
            case 32:{
                // calibration.
                AddConstantX(TemporalArr,RememberInv[w][0]);
            break;
            }
            case 33:{
                setPlotToZero(TemporalArr,AllActions[w][1].toInt());
            break;
            }
            case 34:{
                DeleteExceptCol(TemporalArr,AllActions[w][1].toInt());
            break;
            }
            case 35:{
                averageAllCols(TemporalArr);
            break;
            }
            case 36:{
                RTheta(TemporalArr,AllActions[w][1].toInt(),AllActions[w][2].toInt());
            break;
            }
            case 37:{
                XY(TemporalArr,AllActions[w][1].toInt(),AllActions[w][2].toInt());
            break;
            }
            case 38:{
                AddReference(TemporalArr, q, AllActions[w][1].toInt(), AllActions[w][2].toInt());
            break;
            }
            case 39:{
                MultiplyReference(TemporalArr, q, AllActions[w][1].toInt(), AllActions[w][2].toInt());
            break;
            }
            case 40:{
                SubtractReference(TemporalArr, q, AllActions[w][1].toInt(), AllActions[w][2].toInt());
            break;
            }
            case 41:{
                DivideReference(TemporalArr, q, AllActions[w][1].toInt(), AllActions[w][2].toInt());
            break;
                }
            case 42:{
                MultConstyAreaCol1(TemporalArr);
            break;
            }
            case 43:{
                MultConstyMaxCol1(TemporalArr);
            break;
            }
            case 44:{
                MultiplyConstantX(TemporalArr,RememberInv[w][0]);
            break;
            }

//                AllActions[AllActions.size()-1].resize(1);
//                AllActions[AllActions.size()-1][0]=17;
//                RememberInv[RememberInv.size()-1][0]=VerticalShift

        }
    }
}

void MainWindow::PerformCodeActions(){
    for(int q =0; q<AllData[0].size(); q++){        // Should be running over all files now...
        int TempCol =2;                             // Working column, default 1.
        int TempColRef = 2;                         // Reference column, default 1.
        RunOverActions(AllData[0][q], q, TempCol, TempColRef);
    }
    plotall();
}

void MainWindow::SaveAll(QStringList fitxers, QStringList fitxersSavedAt){
   // QMessageBox::information(this, "Information", QString("The size of actions is %1 and size of rememberInv is %2") .arg(AllActions.size()) .arg(RememberInv.size()));
   // QMessageBox::information(this, "Information", QString("Operation should be 2, %1, in col. [1], %2 and summed value is %3") .arg(AllActions[0][0].toInt()) .arg(AllActions[0][1].toInt()) .arg(AllActions[0][2].toDouble()));


  //  AllActions[AllActions.size()-1][0]=2;
  //  AllActions[AllActions.size()-1][1]=columnPlot;
  //  RememberInv[RememberInv.size()-1][0]=result;

    QVector<QVector<QVector<QVector<double>>>> TemporalArr;
    int error =0;

    for(int q=0; q<fitxers.size(); q++){            // Run over all files
        TemporalArr.resize(1);
        error += getOneData(fitxers.at(q), TemporalArr);     // Get TemporalArr[0][0][col][rows] IDEALLY should call getData, but it was faster to make a new function.
        int TempCol =2;                             // Working column, default 1.
        int TempColRef = 2;                         // Reference column, default 1.

        RunOverActions(TemporalArr[0][0], q, TempCol, TempColRef);

        //SAVE FILE HERE***********************************************************************************************
        SaveData(fitxersSavedAt.at(q), TemporalArr);
        TemporalArr.clear();                        // TemporalArr is reset.
    }

    if (error >=1){
        QMessageBox::information(this,"Information", QString("Warning: Empty lines or lines with invalid data have been detected in %1 files. Saved results automatically corrected some of the errors in the files.") .arg(error) );
    }
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
                    deletedLines = cleanDatafile(inputnames[q], exportnames[q], AllFormatActions[w][1].toInt());
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
                case 11:{
                    startValue(inputnames[q], exportnames[q], AllFormatActions[w][1].toDouble());
                    break;
                }
            }
        }
        rememberExt.clear();
       // QMessageBox::information(this, "Information", QString("Import are now: \n\n%1") .arg(importnames[0]) );
    }
}

void MainWindow::SaveData(QString fitxer, QVector<QVector<QVector<QVector<double>>>> data){ //Note!! It only saves from first datafile! [0][0]!

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

void MainWindow::SaveSingleData(QString fitxer, QVector<QVector<double>> data){ //Note!! It only saves from first datafile! [0][0]!

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
//        qDebug() << QString("the last X saved element was %1") .arg(data[0][data[0].size()-1]);
}

bool MainWindow::checkBackupExists(QString openedNames){
    QFileInfo inputprelputinfo(openedNames);
    QString temp = inputprelputinfo.path() + "/backup/" + inputprelputinfo.completeBaseName() + "." + inputprelputinfo.suffix();
    QFileInfo inputinfo(temp);
    if(QDir(inputinfo.path()).exists()){ //If the output folder doesnt exist we create it.
        return true;
    }else{
        return false;
    }
}

//*********MISCELLANEOUS FUNCTIONS***************

double MainWindow::getFirstX(QString file){
    QVector<QVector<QVector<QVector<double>>>> TempArr;
    TempArr.resize(1);

    if (getOneData(file, TempArr)==0){ // If there's no error. BTW. this should create TempArr.
        return TempArr[0][0][0][0];
    }else{
        qDebug() << "Error in startValue function, couldnt get one data.";
        return 0;
    }
}

double MainWindow::getLastX(QString file){
    QVector<QVector<QVector<QVector<double>>>> TempArr;
    TempArr.resize(1);

    if (getOneData(file, TempArr)==0){ // If there's no error. BTW. this should create TempArr.
        return TempArr[0][0][0][TempArr[0][0][0].size()-1];
    }else{
        qDebug() << "Error in startValue function, couldnt get one data.";
        return 0;
    }
}

std::istream& safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case std::streambuf::traits_type::eof():
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

void MainWindow::getData(QStringList fitxer, QVector<QVector<QVector<QVector<double>>>> &InputData){ //Will create AllData[][i][][] with i= max num of plotable files.
    QString WorkingFitxer;
    int error =0;
    InputData.clear();
    InputData.resize(undo+1); //Because it must be able to undo up to 5 times.
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

                    // while (obre >> line) {                  //Run over rows
                     // while (std::safeGetline(obre, line).eof()) {                  //Run over rows
       // while(!obre.eof()){
        //    inStream >> data;
        //qDebug() << QString("now line is %1") .arg(QString::fromStdString(line));
        /*while(!obre.eof()){
            int line_int;
            obre >> line_int;
            stringstream ss;
            ss << line_int;
            string str = ss.str();*/

        std::string line;
        while (std::getline(obre, line)) {                                              //Run over rows
            //qDebug() << QString("now line is %1") .arg(QString::fromStdString(line));
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
        if(line.empty()){ //checks if last line is empty.
            error=1;
            m--;
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

            //QMessageBox::information(this,"Information", QString("Number of cols is %1 and rows is %2") .arg(ColNums(fitxer)) .arg(RowNums(fitxer)+1) );

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
        //qDebug() << QString("the last X element is %1") .arg(InputData[0][InputData[0].size()-1]);
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
    ColumnMinSize=FileColNums[1]; // I decided to uptdate ColumnMinSize because it is a very important variable.
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

int MainWindow::RawLineNumbers(QString fitxer){
    int num = 0;
    QByteArray ba = fitxer.toLocal8Bit(); //convert fitxer into char c_str2
      const char *c_str2 = ba.data();
    ifstream ifs(c_str2, ios::in); // Input
        std::string line;
        while (std::getline(ifs, line)) {                  //Run over rows
             num++;
          }
    ifs.close();
    RowSize=num-1;
    return RowSize;
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

bool MainWindow::getDoubleNum(double &num1, QString message){
    QString text;
    bool error1 = 0;
    bool ok;

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

        }else{ // Cancell is pressed.
            return false;
        }
    }
    num1 = text.toDouble();
    return true;
}

bool MainWindow::getIntNum(int &num1, int min, int max, QString message){
    QString text;
    bool error1 = 0;
    bool ok;

    while(!error1){
        text = QInputDialog::getText(this, "Input", message, QLineEdit::Normal,"",&ok);
        if(ok){
            if(text != "0"){
                error1=text.toInt();
                if(!error1){
                    QMessageBox::warning(this, "Error", QString("No valid number was introduced.\nIntroduce a valid integer number such as 1.\nPlease, try again."));
                }
            }else{
                error1=1;
            }

        }else{ // Cancell is pressed.
            return false;
        }
    }
    num1 = text.toDouble();
    return true;
}

bool MainWindow::checkFileNames(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return false;
    }else{
        return true;
    }
}

bool MainWindow::checkRefFiles(){
    if(referencenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first add reference files in 'Columns and files'->'Add Reference files' to proceed with this action.") );
        return false;
    }else{
        return true;
    }
}

bool MainWindow::checkRefNumber(){
    if(referencenames.size()==1 || referencenames.size() == filenames.size()){
        return true;
    }else{
        QMessageBox::warning(this, "Warning", QString("The number of reference files (currently %1) must be either one or equal to the number of opened files (i.e. %2).\nTo proceed with this action, open a proper number of reference files.") .arg(referencenames.size()) .arg(filenames.size()) );
        return false;
    }
}

bool MainWindow::equalDoubles(double double1, double double2, double step, double tolerance){
    if(fabs(double1-double2) < tolerance*step){
        return true;
    }else{
       // QMessageBox::warning(this, "Warning", QString("%1 and %2 are not equal.") .arg(double1) .arg(double2) );
       // QMessageBox::warning(this, "Warning", QString("Because the difference, %1, is larger than 0.1 times the step %2.") .arg(abs(double1-double2)) .arg(double2) );
        return false;
    }
}

//********* COLUMNS AND FILES  ***************

bool MainWindow::checkOperateRef(){
    if(!checkFileNames() || !checkRefFiles() || !checkRefNumber()){ // We check if filenames or referencenames are empty and if number of files is proper to make operations between them.
        return false;
    }else{
        return true;
    }
}

bool MainWindow::checkProperRanges(){

    double minFilenames = FindMinFile(filenames.at(0));
    double maxFilenames = FindMaxFile(filenames.at(0));
    int fileRows = RowNums(filenames.at(0));
    double fileRowsDouble = 1.0*fileRows;
    double tolerance = 0.1; // This tolerance parameter is used to compare double numbers.

    // equalDoubles is used to compare doubles in an effective way, since c++ sucks at it.
    for(int q =0; q< filenames.size(); q++){ // run over filenames
        if( !equalDoubles(FindMinFile(filenames.at(q)), minFilenames, (maxFilenames-minFilenames)/fileRowsDouble, tolerance) || !equalDoubles(FindMaxFile(filenames.at(q)), maxFilenames, (maxFilenames-minFilenames)/fileRowsDouble, tolerance) ){
            QMessageBox::warning(this, "Warning", QString("To perform this action, all opened files must exhibit the same X ranges (column 1).\nIt was detected that the range some opened files (%1, %2) is not from %3 to %4.\nNo action was performed.") .arg(FindMinFile(filenames.at(q))) .arg(FindMaxFile(filenames.at(q))) .arg(minFilenames) .arg(maxFilenames) );
            return false;
        }
        if(RowNums(filenames.at(q))!=fileRows){
            QMessageBox::warning(this, "Warning", QString("To perform this action, all opened files must share the same number of data rows.\nIt was detected that the number of some opened files is not  %1.\nNo action was performed.") .arg(fileRows) );
            return false;
        }
    }

    for(int w=0; w< referencenames.size();w++){ // run over referencenames
        if( !equalDoubles(FindMinFile(referencenames.at(w)), minFilenames, (maxFilenames-minFilenames)/fileRowsDouble, tolerance) || !equalDoubles(FindMaxFile(referencenames.at(w)), maxFilenames, (maxFilenames-minFilenames)/fileRowsDouble, tolerance) ){
            QMessageBox::warning(this, "Warning", QString("To perform this action, all reference file(s) must exhibit the same X ranges (column 1) as the opened files.\nIt was detected that the range some reference file(s) is not from %1 to %2./nNo action was performed.\n(tip: The range of Col 1 of files and reference files can be tailored from the 'Format'->'Modify datapoints' menu options.") .arg(minFilenames) .arg(maxFilenames) );
            return false;
        }
        if(RowNums(referencenames.at(w))!=fileRows){
            QMessageBox::warning(this, "Warning", QString("To perform this action, all reference file(s) must share the same number of data rows as the opened files.\nIt was detected that the number of some reference file(s) is not  %1.\nNo action was performed.\n(tip: The range of Col 1 of files and reference files can be tailored from the 'Format'->'Modify datapoints' menu options.)") .arg(fileRows) );
            return false;
        }
    }
    return true;
}

bool MainWindow::checkRefColumn(){
    int minCols = ColNums(referencenames.at(0));

    for(int q=0; q< referencenames.size(); q++){
        if(ColNums(referencenames.at(q))<minCols){
            minCols = ColNums(referencenames.at(q));
        }
    }

    if(minCols<2){
        QMessageBox::warning(this, tr("Warning!"), tr("Operations cannot be performed due to the presence of corrupted reference files.\n(Tip #1: Check that there are no empty datafiles, and their numbers contain dots instead of commas as decimals without thousand separators)\n(Tip #2: In order to modify the format from 'Format' options first import files from 'Import datafile(s)')") );
        return false;
    }else if(minCols==2){
        referenceCol =2;
    }else{        
        if(!getIntNum(referenceCol, 2,minCols, QString("Introduce column number to be used as a reference, from 2 to %1") .arg(minCols))){
            ui->statusbar->showMessage("No reference column was selected.",10000);
            return false;
        }
        //referenceCol = QInputDialog::getInt(this, "Input",QString("Introduce column number to be used as a reference, from 2 to %1") .arg(minCols),1,2,minCols);
    }
    return true;
}

void MainWindow::addFiles(QString outFilename, int col, double average){
    QVector<QVector<double>> VecOut;

    // 1- I need to resize VecOut;
    VecOut.resize(2);

    VecOut[0].resize(AllData[0][0][0].size());
    VecOut[1].resize(AllData[0][0][0].size());

    for(int w=0; w<VecOut[0].size();w++){ // we run over rows.
        VecOut[0][w]=0.0;                   // We initialise VecOut
        VecOut[1][w]=0.0;                   // We initialise VecOut
    }

    // 2- I need to open all files (and then close), one by one, and sum the correct column to VecOut
    QVector<QVector<QVector<QVector<double>>>> TempArr;
    TempArr.resize(1);

    for(int q = 0; q<filenames.size(); q++){   //run over files.
        if (getOneData(filenames[q], TempArr)==0){ // If there's no error. BTW. this should create TempArr.
           for(int e =0; e<VecOut[0].size(); e++){ // run over rows
                    VecOut[0][e]=TempArr[0][0][0][e];   // this is a bit repetitive but code lines are saved.
                    VecOut[1][e]=VecOut[1][e]+TempArr[0][0][col-1][e];
            }
        }else{
            qDebug() << "Error in startValue function, couldnt get one data.";
        }
    }

    // 3- Make average

    for(int e =0; e<VecOut[0].size(); e++){ // run over rows
             VecOut[1][e]=VecOut[1][e]/average;   // this is a bit repetitive but code lines are saved.
     }

    // 4- I need to open and writte answer to the new file with proper directory direction.

    QFileInfo filenamesinfo(filenames.at(0));
    QString temp = filenamesinfo.path() + "/" + outFilename + "." + filenamesinfo.suffix();
    SaveSingleData(temp, VecOut);
}

void MainWindow::averageAllCols(QVector<QVector<double>> &vec){
    vec.resize(vec.size()+1); // we create a new column
    vec[vec.size()-1].resize(vec[0].size());
    double average =0;

    for(int q =0; q<vec[0].size(); q++){ // run over rows
        for(int w=1; w< vec.size()-1; w++){ // run over columns
            average = average + vec[w][q];
        }
        vec[vec.size()-1][q] = average/(vec.size()-2);
        average =0;
    }
}

int MainWindow::DeleteExceptCol(QVector<QVector<double>> &vec, int result){
    int deletedCols = vec.size();
    for(int w=0; w<vec[0].length();w++){    // Run over rows
        vec[1][w]=vec[result-1][w];
    }
    vec.resize(2);
    return deletedCols - 2;
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

void MainWindow::AddReference(QVector<QVector<double>> &vec, int fileNumber, int colPlot, int refCol){

    if(colPlot==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.
           // if(r!=(refCol-1)){      // we ignore the reference column.
                QVector<QVector<double>> refData;
                refData.resize(1);
                if(referencenames.size()==1){
                    getOneSingleData(referencenames.at(0), refData);
                }else{
                    getOneSingleData(referencenames.at(fileNumber), refData);
                }

                for (int w = 0; w < vec[0].length(); w++){
                    vec[r][w] = vec[r][w]+refData[refCol-1][w];
                }
           // }
        }
    }else{
        QVector<QVector<double>> refData;
        refData.resize(1);
        if(referencenames.size()==1){
            getOneSingleData(referencenames.at(0), refData);
        }else{
            getOneSingleData(referencenames.at(fileNumber), refData);
        }

        for (int w = 0; w < vec[0].length(); w++){
            vec[colPlot-1][w] = vec[colPlot-1][w]+refData[refCol-1][w];
        }
    }
}

void MainWindow::MultiplyReference(QVector<QVector<double>> &vec, int fileNumber, int colPlot, int refCol){

    if(colPlot==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.
          //  if(r!=(refCol-1)){
                QVector<QVector<double>> refData;
                refData.resize(1);
                if(referencenames.size()==1){
                    getOneSingleData(referencenames.at(0), refData);
                }else{
                    getOneSingleData(referencenames.at(fileNumber), refData);
                }

                for (int w = 0; w < vec[0].length(); w++){
                    vec[r][w] = vec[r][w]*refData[refCol-1][w];
                }
           // }
        }
    }else{
        QVector<QVector<double>> refData;
        refData.resize(1);
        if(referencenames.size()==1){
            getOneSingleData(referencenames.at(0), refData);
        }else{
            getOneSingleData(referencenames.at(fileNumber), refData);
        }

        for (int w = 0; w < vec[0].length(); w++){
            vec[colPlot-1][w] = vec[colPlot-1][w]*refData[refCol-1][w];
        }
    }
}

void MainWindow::SubtractReference(QVector<QVector<double>> &vec, int fileNumber, int colPlot, int refCol){

    if(colPlot==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.
          //if(r!=(refCol-1)){
              QVector<QVector<double>> refData;
              refData.resize(1);
              if(referencenames.size()==1){
                  getOneSingleData(referencenames.at(0), refData);
              }else{
                  getOneSingleData(referencenames.at(fileNumber), refData);
              }

              for (int w = 0; w < vec[0].length(); w++){
                  vec[r][w] = vec[r][w]-refData[refCol-1][w];
              }
          //}
        }
    }else{
        QVector<QVector<double>> refData;
        refData.resize(1);
        if(referencenames.size()==1){
            getOneSingleData(referencenames.at(0), refData);
        }else{
            getOneSingleData(referencenames.at(fileNumber), refData);
        }

        for (int w = 0; w < vec[0].length(); w++){
            vec[colPlot-1][w] = vec[colPlot-1][w]-refData[refCol-1][w];
        }
    }
}

void MainWindow::DivideReference(QVector<QVector<double>> &vec, int fileNumber, int colPlot, int refCol){

    if(colPlot==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.
           // if(r!=(refCol-1)){
                QVector<QVector<double>> refData;
                refData.resize(1);
                if(referencenames.size()==1){
                    getOneSingleData(referencenames.at(0), refData);
                }else{
                    getOneSingleData(referencenames.at(fileNumber), refData);
                }

                for (int w = 0; w < vec[0].length(); w++){
                    if(refData[refCol-1][w]!=0.0){ // to prevent dividing by zero.
                        vec[r][w] = vec[r][w]/refData[refCol-1][w];
                    }
                }
           // }
        }
    }else{
        QVector<QVector<double>> refData;
        refData.resize(1);
        if(referencenames.size()==1){
            getOneSingleData(referencenames.at(0), refData);
        }else{
            getOneSingleData(referencenames.at(fileNumber), refData);
        }

        for (int w = 0; w < vec[0].length(); w++){
            if(refData[refCol-1][w]!=0.0){ // to prevent dividing by zero.
                vec[colPlot-1][w] = vec[colPlot-1][w]/refData[refCol-1][w];
            }
        }
    }
}

//*********ANALYSIS FUNCTIONS***************

void MainWindow::AppendStrings(QString &code, QVector<QVector<QString>> Actions){
    if(!Actions[0].isEmpty()){
        code.append(Actions[0][0]);
        if(Actions[0].size()>1){
            for(int w=1; w< Actions[0].size();w++){
                code.append(",");
                code.append(Actions[0][w]);
            }
        }
    }
    if(Actions.size()>1){
        for(int q =1; q< Actions.size(); q++){
            code.append(";");
            if(!Actions[q].isEmpty()){
                code.append(Actions[q][0]);
                if(Actions[q].size()>1){
                    for(int w=1; w< Actions[q].size();w++){
                        code.append(",");
                        code.append(Actions[q][w]);
                    }
                }
            }
        }
    }
}

void MainWindow::ReadCode(QString code){

    QStringList codeList = code.split(":");
    if(codeList.size()<2 || codeList.size()>3){
        QMessageBox::warning(this, "Warning", QString("Invalid code, wrong number of colons.\n(Tip #1: Make sure you copy the whole generated code)\n(Tip #2: Modifying a code is not advised since it can cause errors)") );
        return;
    }


    // 1- First part of the code is the columnPlot and other 'global' important variables. (consider adding more in the future, if needed).
    QStringList SubCodeList = codeList.at(0).split(";");
    //QString colPlot = QString("%1;%2;%3") .arg(columnPlot) .arg(ColumnMinSize) .arg(plotNumLim);

    // 1.1- We should not set columnPlot to the ending columnPlot of previous actions since it could yield problems. That's why we only use a temp, local variable.
    // Hence, we only check if multicolumnPlot is ON or OFF.
    // The value of columnPlot in the code can be used to check if after performing the code, we get the same value and everything went OK.
    // If it didn't, a Warning message should tell the user that something went wrong.
    int tempColPlot = SubCodeList.at(0).toInt();
    // columnPlot = SubCodeList.at(0).toInt();
    if(tempColPlot ==0){
        //AllColumns = true;
        columnPlot = 0;
    }else{
        //AllColumns = false;
    }

    // 1.2- Actions will modify this value, hence we might only use this final value to check that all code Actions yielded equal results. We comment it.
    // ColumnMinSize = SubCodeList.at(1).toInt();

    // 1.3- We need to recover this value because it is not set in any Action.
    plotNumLim = SubCodeList.at(2).toInt();

    // 1.4- Similarly we need to know if we are working with multiple visible files at the same time or not since this option is not set in any Action
    int tempFlag1 = SubCodeList.at(3).toInt();
    if(tempFlag1==1){
        flag1 = true;
    }else{
        flag1 = false;
    }

    // 2- Second part of the code is AllActions.
    QStringList ActionsList = codeList.at(1).split(";");

    AllActions.resize(ActionsList.size());
    for(int q =0; q<ActionsList.size(); q++){
        QStringList SubActionsList = ActionsList.at(q).split(",");
        AllActions[q].resize(SubActionsList.size());
        for(int w=0; w<SubActionsList.size();w++){
            AllActions[q][w]= SubActionsList.at(w);
        }
        SubActionsList.clear(); // probably unnecesary but we don't need it anymore.
    }
    ActionsList.clear(); // probably unnecesary but we don't need it anymore.

    // 3- Third and last part of the code is RememberInv.
    if(codeList.size()==3){
        QStringList RememberList = codeList.at(2).split(";");

        RememberInv.resize(RememberList.size());
        for(int q=0; q< RememberList.size(); q++){
            QStringList SubRememberList = RememberList.at(q).split(",");
            RememberInv[q].resize(SubRememberList.size());
            for(int w=0; w<SubRememberList.size();w++){
                if(!SubRememberList.at(w).isEmpty()){
                    RememberInv[q][w] = SubRememberList.at(w).toDouble();
                }
            }
        }
    }
}

void MainWindow::CheckExectuedCode(QString code){
    bool errors = false;

    QStringList codeList = code.split(":");
    QStringList SubCodeList = codeList.at(0).split(";");

    int tempColPlot = SubCodeList.at(0).toInt();
    if(tempColPlot !=columnPlot){
        QMessageBox::information(this, "Information", QString("Current columnPlot is %1, should be %2.") .arg(columnPlot) .arg(tempColPlot) );
        errors = true;
        columnPlot=tempColPlot;
    }

    int tempColMinSize = SubCodeList.at(1).toInt();
    if(tempColMinSize != ColumnMinSize){
        QMessageBox::information(this, "Information", QString("Current ColumnMinSize is %1, should be %2.") .arg(ColumnMinSize) .arg(tempColMinSize) );
        errors = true;
        ColumnMinSize=tempColMinSize;
    }

    if(errors){
        QMessageBox::information(this, "Information", QString("Some errors were detected during execution of the code."));
    }

}

QString MainWindow::GenerateCode(){
    // The code has the form:       columnPlot : AllActions[q][w] : RememberInv[q][w]
    // Where AllActions and RememberInv are separated by ; each [q] and by , for each [w].
    // To decode it, columnPlot will need to be converted back to int, and RememberInv back to double.
    // Note that RememberInv will leave many empty [q], seen as ;;.

    QString code;
    QVector<QVector<QString>> RememberInvString;
    QString colPlot = QString("%1;%2;%3;%4") .arg(columnPlot) .arg(ColumnMinSize) .arg(plotNumLim) .arg(flag1);
    code.append(colPlot+":");

    AppendStrings(code, AllActions);

    if(!RememberInv.isEmpty()){
        if(RememberInv.size()>0){
            code.append(":");

            RememberInvString.resize(RememberInv.size());
            for(int q=0; q< RememberInv.size(); q++){
                RememberInvString[q].resize(RememberInv[q].size());
                for(int w=0; w<RememberInv[q].size(); w++){
                    RememberInvString[q][w]=QString("%1") .arg(RememberInv[q][w]);
                }
            }
          //  QMessageBox::information(this, "Information", QString("No peta 4. Dimensions of RememberInvString are [%1][%2]") .arg(RememberInvString.size()) .arg(RememberInvString[0].size()) );

            AppendStrings(code, RememberInvString);
           // QMessageBox::information(this, "Information", QString("No peta 5") );
        }
    }

    return code;
}

void MainWindow::XY(QVector<QVector<double>> &vec, int R, int T){
    vec.resize(vec.size()+2);
    vec[vec.size()-2].resize(vec[0].size());
    vec[vec.size()-1].resize(vec[0].size());
    for (int w = 0; w < vec[0].length(); w++){ // run over rows.
       vec[vec.size()-2][w] = vec[R-1][w]*cos(vec[T-1][w]);     // X
       vec[vec.size()-1][w] = vec[R-1][w]*sin(vec[T-1][w]);     // Y
    }
}

void MainWindow::RTheta(QVector<QVector<double>> &vec, int X, int Y){
    vec.resize(vec.size()+2);
    vec[vec.size()-2].resize(vec[0].size());
    vec[vec.size()-1].resize(vec[0].size());
    for (int w = 0; w < vec[0].length(); w++){ // run over rows.
       vec[vec.size()-2][w] = pow(pow(vec[X-1][w],2)+pow(vec[Y-1][w],2), 0.5);   // R
       //Now we calculate Theta.
       //Theta for quadrants I and IV
       if(vec[X-1][w]>0){
               vec[vec.size()-1][w] = atan(vec[Y-1][w]/vec[X-1][w]);     // Theta
               //Theta for quadrants I and IV
       }else if(vec[X-1][w]<0) {
           vec[vec.size()-1][w] = atan(vec[Y-1][w]/vec[X-1][w])+PI;     // Theta
       }else{       // X = 0 could yield error of dividing by zero.
           if(vec[Y-1][w] == 0.0){ // this should never happen, it could cause and error so i make it break the for.
               vec[vec.size()-1][w] = 0;
               break;
           }else if(vec[Y-1][w]>0){
               vec[vec.size()-1][w] = PI/2;
           }else{
               vec[vec.size()-1][w] = 3*PI/2;
           }
       }
    }
}

void MainWindow::Exponentiate(QVector<QVector<double>> &vec, int colPlot, double result){
    if(colPlot==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.
            for (int w = 0; w < vec[0].length(); w++){
               vec[r][w] = pow(vec[r][w],result);
            }
        }
    }else{
        for (int w = 0; w < vec[0].length(); w++){
           vec[colPlot-1][w] = pow(vec[colPlot-1][w],result);
        }
    }
}

void MainWindow::logarithmX(QVector<QVector<double>> &vec, double result){
    for (int w = 0; w < vec[0].length(); w++){
       vec[0][w] = log10(vec[0][w])/log10(result);
    }
}

void MainWindow::logarithm(QVector<QVector<double>> &vec, int colPlot, double result){

    if(colPlot==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.
            for (int w = 0; w < vec[0].length(); w++){
               vec[r][w] = log10(vec[r][w])/log10(result);
            }
        }
    }else{
        for (int w = 0; w < vec[0].length(); w++){
           vec[colPlot-1][w] = log10(vec[colPlot-1][w])/log10(result);
        }
    }
}

void MainWindow::ExponentiateX(QVector<QVector<double>> &vec, double result){
    for (int w = 0; w < vec[0].length(); w++){
        vec[0][w] = pow(vec[0][w],result);
    }
}

void MainWindow::AddConstantX(QVector<QVector<double>> &vec, double result){
    for (int w = 0; w < vec[0].length(); w++){
        vec[0][w] = vec[0][w]+result;
    }
}

void MainWindow::MultiplyConstantX(QVector<QVector<double>> &vec, double result){
    for (int w = 0; w < vec[0].length(); w++){
        vec[0][w] = vec[0][w]*result;
    }
}

void MainWindow::integrate(QVector<QVector<double>> &vec, int colPlot){

    if(colPlot==0){
        for(int q =1; q<vec.length();q++){ // run over columns.

            if(vec[0][0]>vec[0][vec[0].size()-1]){ //decreasing order
                for (int w = vec[0].length()-2; w >= 0; w--){  // We run over rows.
                    vec[q][w] = vec[q][w]+vec[q][w+1];
                }
            }else{  // increasing order
                for (int w = 1; w < vec[0].length(); w++){  // We run over rows.
                    vec[q][w] = vec[q][w]+vec[q][w-1];
                }
            }

        }

    }else{

    if(vec[0][0]>vec[0][vec[0].size()-1]){ //decreasing order
        for (int w = vec[0].length()-2; w >= 0; w--){  // We run over rows.
            vec[colPlot-1][w] = vec[colPlot-1][w]+vec[colPlot-1][w+1];
        }
    }else{  // increasing order
        for (int w = 1; w < vec[0].length(); w++){  // We run over rows.
            vec[colPlot-1][w] = vec[colPlot-1][w]+vec[colPlot-1][w-1];
        }
    }

    }

}

void MainWindow::differentiate(QVector<QVector<double>> &vec, int colPlot){

    if(colPlot==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.
            for (int w = 0; w < vec[0].length()-1; w++){  // We run over rows.
                vec[r][w] = (vec[r][w+1]-vec[r][w])/(vec[0][w+1]-vec[0][w]);
            }
            // The last value is equal to the one before the end.
            vec[r][vec[0].length()-1] = vec[r][vec[0].length()-2];
        }
    }else{
        for (int w = 0; w < vec[0].length()-1; w++){  // We run over rows.
            vec[colPlot-1][w] = (vec[colPlot-1][w+1]-vec[colPlot-1][w])/(vec[0][w+1]-vec[0][w]);
        }
        // The last value is equal to the one before the end.
        vec[colPlot-1][vec[0].length()-1] = vec[colPlot-1][vec[0].length()-2];
    }
}

void MainWindow::LaplacianSmooth(QVector<QVector<double>> &vec, int colPlot, int times){

    if(colPlot==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.
            for(int q=0;q<times;q++){
                for (int w = 1; w < vec[0].length()-1; w++){  // We run over rows.
                    vec[r][w] = (vec[r][w-1]+vec[r][w+1])/2;
                }
            }
        }
    }else{
        for(int q=0;q<times;q++){
            for (int w = 1; w < vec[0].length()-1; w++){  // We run over rows.
                vec[colPlot-1][w] = (vec[colPlot-1][w-1]+vec[colPlot-1][w+1])/2;
            }
        }
    }
}

void MainWindow::Filter(QVector<QVector<double>> &a1, int Col, double lowfreq, double highfreq){

    if(Col==0){
        for(int r=1;r<a1.size();r++){   // Run over cols.
            int lines=a1[0].size();
            //QMessageBox::information(this, "Information", QString("Lines are %1") .arg(lines) );
            complex<double> a[2][lines];
            complex<double> b[lines];
             for(int i=0;i<lines;i++){
             b[i]=0;
             a[0][i]=a1[0][i];
             a[1][i]=a1[r][i];
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
                a1[r][f]= real(c[f]);
                //a1[Col-1][f]= real(b[f]);
            }
        }
    }else{
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
}

void MainWindow::AddConstant(QVector<QVector<double>> &vec,int colPlot, double result){
    if(colPlot==0){
        for(int q = 1; q < vec.length(); q++){ // run over all columns.
            for (int w = 0; w < vec[0].length(); w++){
                vec[q][w] = vec[q][w]+result;
            }
        }
    }else{
        for (int w = 0; w < vec[0].length(); w++){
            vec[colPlot-1][w] = vec[colPlot-1][w]+result;
        }
    }
}

void MainWindow::KKA(QVector<QVector<double>> &vec, int columnPlot){

    if(columnPlot==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.
            QVector<QVector<double>> vecTemp; //temporal vector
            vecTemp.resize(1);                                   //We add one column for the answer. On secon thought, I think it is better to overwritte the files, like in Filter.
            vecTemp[0].resize(vec[0].size());
            double integral;
            double deltaE = abs(vec[0][0]-vec[0][1]);
            for(int q=0; q<vec[0].size();q++){
                integral = 0.0;
                for(int w=0; w<vec[0].size();w++){
                    if(q!=w){
                        integral = integral+2*vec[0][q]*vec[r][w]*deltaE/(PI*(pow(vec[0][q],2)-pow(vec[0][w],2)));
                    }
                }
                //vec[vec.size()-1][q]=pow(pow(vec[columnPlot-1][q],2)+pow(integral,2),0.5);
                vecTemp[0][q]=pow(pow(vec[r][q],2)+pow(integral,2),0.5);
            }
            for(int e=0; e<vec[0].size();e++){
                vec[r][e] = vecTemp[0][e];
            }
        }
    }else{
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
}

void MainWindow::SubtractColumns(QVector<QVector<double>> &vec,int colPlot,int RefCol){
    int initialVecSize = vec.size(); //because this gets larger per iteration.

    if(colPlot==0){
        for(int w=1; w<initialVecSize;w++){ // run over columns
            vec.resize(vec.size()+1);                                   //We add one column for the answer
            vec[vec.size()-1].resize(vec[0].size());                    //We create space for all rows in new column.
            for(int q=0;q<vec[0].size();q++){                           //Run over rows
                vec[vec.size()-1][q]=vec[w][q]-vec[RefCol-1][q];  //Subtracting columns
            }
        }
    }else{        
        vec.resize(vec.size()+1);                                   //We add one column for the answer
        vec[vec.size()-1].resize(vec[0].size());                    //We create space for all rows in new column.
        for(int q=0;q<vec[0].size();q++){                           //Run over rows
            vec[vec.size()-1][q]=vec[colPlot-1][q]-vec[RefCol-1][q];  //Subtracting columns
        }
    }
}

void MainWindow::AddColumns(QVector<QVector<double>> &vec,int colPlot,int RefCol){
    int initialVecSize = vec.size(); //because this gets larger per iteration.

    if(colPlot==0){
        for(int r=1;r<initialVecSize;r++){   // Run over cols.
           // if(r!=(RefCol-1)){          // we ignore the reference column

                vec.resize(vec.size()+1);                                   //We add one column for the answer
                vec[vec.size()-1].resize(vec[0].size());                    //We create space for all rows in new column.
                for(int q=0;q<vec[0].size();q++){                           //Run over rows
                    vec[vec.size()-1][q]=vec[r][q]+vec[RefCol-1][q];  //Adding columns
                }
           // }
        }
    }else{
        vec.resize(vec.size()+1);                                   //We add one column for the answer
        vec[vec.size()-1].resize(vec[0].size());                    //We create space for all rows in new column.
        for(int q=0;q<vec[0].size();q++){                           //Run over rows
            vec[vec.size()-1][q]=vec[colPlot-1][q]+vec[RefCol-1][q];  //Adding columns
        }
    }
}

bool MainWindow::DivideColumns1Column(QVector<QVector<double>> &vec,int colPlot,int RefCol){
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
       // QMessageBox::information(this, "Warning!", QString("Some denominator values equal 0. Since cannot divide by 0, result is set to 0.") );
        return true;
    }else{
        return false;
    }
}

void MainWindow::DivideColumns(QVector<QVector<double>> &vec,int colPlot,int RefCol){
    int initialVecSize = vec.size(); //because this gets larger per iteration.
    bool DivideZero = false;
    if(colPlot==0){
        for(int w =1; w<initialVecSize; w++){                      // Run over cols
            if (DivideColumns1Column(vec, w+1, RefCol)){
                DivideZero = true;
            }

        }

    }else{
        if(DivideColumns1Column(vec, colPlot, RefCol)){
            DivideZero = true;
        }
    }
    if(DivideZero){
       QMessageBox::information(this, "Warning!", QString("Some denominator values equal 0. Since cannot divide by 0, result is set to 0.") );
    }

}

void MainWindow::MultiplyColumns(QVector<QVector<double>> &vec,int colPlot,int RefCol){
    int initialVecSize = vec.size(); //because this gets larger per iteration.
    if(colPlot==0){
        for(int w=1; w<initialVecSize;w++){     // Run over columns
            //if(w!=(RefCol-1)){              // Ignoring the reference column
                vec.resize(vec.size()+1);                                   //We add one column for the answer
                vec[vec.size()-1].resize(vec[0].size());                    //We create space for all rows in new column.
                for(int q=0;q<vec[0].size();q++){                           //Run over rows
                  vec[vec.size()-1][q]=vec[w][q]*vec[RefCol-1][q];  //Multiplying columns
                }

          //  }
        }
    }else{
        vec.resize(vec.size()+1);                                   //We add one column for the answer
        vec[vec.size()-1].resize(vec[0].size());                    //We create space for all rows in new column.
        for(int q=0;q<vec[0].size();q++){                           //Run over rows
          vec[vec.size()-1][q]=vec[colPlot-1][q]*vec[RefCol-1][q];  //Multiplying columns
        }
    }
}

void MainWindow::MultConsty(QVector<QVector<double> > &vec, double value, int col){
    for(int q=0;q<vec[col-1].size();q++){
        vec[col-1][q]=vec[col-1][q]*value;
    }
}

void MainWindow::MultConstyAreaCol1(QVector<QVector<double> > &vec){
    double num = Integrate(vec, 2);
    double AreaColq;
    for(int q=2;q<vec.size();q++){//run over columns
        AreaColq = Integrate(vec,q+1);
        for(int w=0;w<vec[q].size();w++){
            vec[q][w] = vec[q][w]*num/AreaColq;
        }
    }
}

void MainWindow::MultConstyMaxCol1(QVector<QVector<double> > &vec){
    double num = Maxy(vec, 2);
    double MaxColq;
    for(int q=2;q<vec.size();q++){//run over columns
        MaxColq = Maxy(vec,q+1);
        for(int w=0;w<vec[q].size();w++){
            vec[q][w] = vec[q][w]*num/MaxColq;
        }
    }
}

void MainWindow::MultConsty1(QVector<QVector<double> > &vec, int col){
    if(col==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.

            double num = Integrate(vec, r+1);
            double value = 1/num;
            for(int q=0;q<vec[r].size();q++){
                vec[r][q]=vec[r][q]*value;
            }
        }
    }else{

        double num = Integrate(vec, col);
        double value = 1/num;
        for(int q=0;q<vec[col-1].size();q++){
            vec[col-1][q]=vec[col-1][q]*value;
        }

    }
}

void MainWindow::MultConsty2(QVector<QVector<double> > &vec, int col){
    if(col==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.
            double num = Maxy(vec, r+1);
            double value = 1/num;
            for(int q=0;q<vec[r].size();q++){ // Run over rows.
                vec[r][q]=vec[r][q]*value;
            }
        }
    }else{
        double num = Maxy(vec, col);
        double value = 1/num;
        for(int q=0;q<vec[col-1].size();q++){
            vec[col-1][q]=vec[col-1][q]*value;
        }

    }
}

void MainWindow::MultConsty3(QVector<QVector<double> > &vec, double value, int col){
    if(col==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.
            for(int q=0;q<vec[r].size();q++){
                vec[r][q]=vec[r][q]*value;
            }
        }
    }else{
        for(int q=0;q<vec[col-1].size();q++){
            vec[col-1][q]=vec[col-1][q]*value;
        }
    }
}

//*********PREPARE DATA***************

void MainWindow::setPlotToZero(QVector<QVector<double>> &vec, int columnPlot){
    if(columnPlot==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.
            double min = FindGlobalMinY(vec, r+1);
            for(int q=0;q<vec[r].size();q++){
                vec[r][q]=vec[r][q]-min;
            }
        }
    }else{
        double min = FindGlobalMinY(vec, columnPlot);
        for(int q=0;q<vec[columnPlot-1].size();q++){
            vec[columnPlot-1][q]=vec[columnPlot-1][q]-min;
        }
    }
}

double MainWindow::FindGlobalMinY(QVector<QVector<double>> vec, int columnPlot){
    double min = vec[columnPlot-1][0];
    for(int q=1;q<vec[columnPlot-1].size();q++){
        if(vec[columnPlot-1][q] < min){
            min = vec[columnPlot-1][q];
        }
    }
    return min;
}

void MainWindow::UnitsEnergy(QVector<QVector<double>> &vec, int previousUnit, int newUnit){
    double factor1, factor2, eV;
    double power1, power2;
    // items << tr("eV") << tr("meV") << tr("cm-1") << tr("THz") << tr("nm") << tr("um");
    switch(previousUnit){ // We convert it to eV
        case 0:{
            factor1 = 1.0;
            power1 = 1.0;
            break;
        }
        case 1:{
            factor1 = 1.0/1000;
            power1 = 1.0;
            break;
        }
        case 2:{
            factor1 = 1.0/8065.54429;
            power1 = 1.0;
            break;
        }
        case 3:{
            factor1 = 1.0/241.79893;
            power1 = 1.0;
            break;
        }
        case 4:{
            factor1 = 1239.84195;
            power1 = -1.0;
            break;
        }
        case 5:{
            factor1 = 1.23984195;
            power1 = -1.0;
            break;
        }
        case 6:{
            factor1 = 1.0/1.602176565e-19;
            power1 = 1.0;
            break;
        }
    }

    switch(newUnit){ // We convert it to desired units
        case 0:{
            factor2 = 1.0;
            power2 = 1.0;
            break;
        }
        case 1:{
            factor2 = 1000.0;
            power2 = 1.0;
            break;
        }
        case 2:{
            factor2 = 8065.54429;
            power2 = 1.0;
            break;
        }
        case 3:{
            factor2 = 241.79893;
            power2 = 1.0;
            break;
        }
        case 4:{
            factor2 = 1239.84195;
            power2 = -1.0;
            break;
        }
        case 5:{
            factor2 = 1.23984195;
            power2 = -1.0;
            break;
        }
        case 6:{
            factor2 = 1.602176565e-19;
            power2 = 1.0;
            break;
        }
    }

    for(int q=0;q<vec[0].size();q++){
        eV = factor1*pow(vec[0][q],power1); // now it is in eV
        vec[0][q]=factor2*pow(eV,power2);
    }

}

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

void MainWindow::BallBaseline1Column(QVector<QVector<double>> &vec, int col, double radii){
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

void MainWindow::BallBaseline(QVector<QVector<double>> &vec, int col, double radii){ //This program wont work for small datavalues with Y below 0.1.
    if(col==0){ // must run over all columns.
        for(int q = 1; q< vec.size(); q++){
           BallBaseline1Column(vec, q+1, radii);
        }
    }else{
        BallBaseline1Column(vec, col, radii);
    }
}

double MainWindow::Integrate(QVector<QVector<double>> &vec, int col){

    if(col==0){
        for(int r=1;r<vec.size();r++){   // Run over cols.
            double result=0;
            for(int q=0; q<vec[r].size()-1; q++){  // run over rows
                result=result+fabs((vec[0][1]-vec[0][0])*(vec[r][q]+vec[r][q+1])/2);
            }
        return result;

        }
    }else{
        double result=0;
        for(int q=0; q<vec[col-1].size()-1; q++){  // run over rows
            result=result+fabs((vec[0][1]-vec[0][0])*(vec[col-1][q]+vec[col-1][q+1])/2);
        }
    return result;
    }
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

    if(col==0){
        for(int r=1;r<vec.size();r++){  // Run over cols.
            double slope = (vec[r][vec[r].size()-1]-vec[r][0])/(vec[0][vec[r].size()-1]-vec[0][0]);
            double intercept = vec[r][0]-slope*vec[0][0];
            for(int q=0; q<vec[r].size();q++){
                vec[r][q]=vec[r][q]-intercept-slope*vec[0][q];
            }
        }
    }else{
        double slope = (vec[col-1][vec[col-1].size()-1]-vec[col-1][0])/(vec[0][vec[col-1].size()-1]-vec[0][0]);
        double intercept = vec[col-1][0]-slope*vec[0][0];
        for(int q=0; q<vec[col-1].size();q++){
            vec[col-1][q]=vec[col-1][q]-intercept-slope*vec[0][q];
        }
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

int MainWindow::spikinator1Column(QVector<QVector<double>> &vec, int points, int col, double sigmas, int modify){
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

int MainWindow::spikinator(QVector<QVector<double>> &vec, int points, int col, double sigmas, int modify){

    int spikes=0;

    if(col==0){ // must run over all columns.
        for(int q = 1; q< vec.size(); q++){
            spikes = spikes + spikinator1Column(vec, points, q+1, sigmas, modify);
        }
    }else{
        spikes = spikinator1Column(vec, points, col, sigmas, modify);
    }

    return spikes;
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
    if(columnPlot==0){
        for(int e=1; e<vec.size(); e++){
            for(int w=0; w<vec[0].size(); w++){ //run over rows.
                vec[e][w]=vec[e][w]+(e-1)*VerticalShift;
            }
        }
    }else{
        for(int w=0; w<vec[columnPlot-1].size(); w++){ //run over rows.
            vec[columnPlot-1][w]=vec[columnPlot-1][w]+q*VerticalShift;
        }
    }
}

void MainWindow::scaleMargins(int colPlot){
    if(filenames.isEmpty()){ //I dont know if this is needed, probably not.
        return;
    }

    Plot_margins.clear();
    Plot_margins.resize(4);
    double min_abs_x, max_abs_x, min_abs_y, max_abs_y;
    double *min_x, *max_x, *min_y, *max_y;

      //  QMessageBox::information(this,"AllData[0] exists", QString("Data starts x= %1 , y= %2") .arg(AllData[0][0][1]) .arg(AllData[0][1][1]));
        min_x = std::min_element(AllData[0][0][0].begin(), AllData[0][0][0].end());
        max_x = std::max_element(AllData[0][0][0].begin(), AllData[0][0][0].end());
        min_y = std::min_element(AllData[0][0][colPlot-1].begin(), AllData[0][0][colPlot-1].end());
        max_y = std::max_element(AllData[0][0][colPlot-1].begin(), AllData[0][0][colPlot-1].end());
         min_abs_x=*min_x;
         max_abs_x=*max_x;
         min_abs_y=*min_y;
         max_abs_y=*max_y;
        //QMessageBox::information(this,"Initial data", QString("x from %1 to %2 and y from %3 to %4") .arg(min_abs_x) .arg(max_abs_x) .arg(min_abs_y) .arg(max_abs_y));

         if (flag1 == 1){ //We are plotting multiple files.
            //for (int i = 1; i< filenames.size();i++){ // run over files.
            for (int i = 1; i< plotNum;i++){ // run over plotted files.
                 //QMessageBox::information(this,"Rescaling", QString("Loop x= %1") .arg(i));
                min_x = std::min_element(AllData[0][i][0].begin(), AllData[0][i][0].end());
                max_x = std::max_element(AllData[0][i][0].begin(), AllData[0][i][0].end());
                min_y = std::min_element(AllData[0][i][colPlot-1].begin(), AllData[0][i][colPlot-1].end());
                max_y = std::max_element(AllData[0][i][colPlot-1].begin(), AllData[0][i][colPlot-1].end());
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

         Plot_margins[0] = min_abs_x;
         Plot_margins[1] = max_abs_x;
         Plot_margins[2] = min_abs_y;
         Plot_margins[3] = max_abs_y;
}

void MainWindow::findMargins(int file, int col){
    Plot_margins.clear();
    Plot_margins.resize(4);
    double *min_x, *max_x, *min_y, *max_y;

    min_x = std::min_element(AllData[0][file][0].begin(), AllData[0][file][0].end());
    max_x = std::max_element(AllData[0][file][0].begin(), AllData[0][file][0].end());
    min_y = std::min_element(AllData[0][file][col].begin(), AllData[0][file][col].end());
    max_y = std::max_element(AllData[0][file][col].begin(), AllData[0][file][col].end());

    Plot_margins[0] = *min_x;
    Plot_margins[1] = *max_x;
    Plot_margins[2] = *min_y;
    Plot_margins[3] = *max_y;
}

void MainWindow::getMyMargins(double &minx, double &maxx, double &miny, double &maxy){
    minx = Plot_margins[0];
    maxx = Plot_margins[1];
    miny = Plot_margins[2];
    maxy = Plot_margins[3];
}

void MainWindow::updateMyMargins(double &minx, double &maxx, double &miny, double &maxy){
    if(Plot_margins[0]<minx){
        minx=Plot_margins[0];
    }
    if(Plot_margins[1]>maxx){
        maxx=Plot_margins[1];
    }
    if(Plot_margins[2]<miny){
        miny=Plot_margins[2];
    }
    if(Plot_margins[3]>maxy){
        maxy=Plot_margins[3];
    }
}

void MainWindow::findGlobalMargins(){
    //Here we have TWO POSSIBLE CASES, depending on wether we are resizing for one or all columns.
    double min_x,max_x,min_y,max_y;

    if(columnPlot ==0){ // CASE 1: We are plotting all columns for a certain number of files.
       // QMessageBox::information(this, "Information", QString("You are in case 3. One or a few files (%1 plotted files) and all columns") .arg(plotNum) );

        findMargins(0, 1); // To get a first values, find margins of file 0 and col 1.
        getMyMargins(min_x,max_x,min_y,max_y);  // Used only for initialization of these doubles.
        for(int q=0; q<currFilePlotNum;q++){ // run over plotted files
            for(int w=1;w<AllData[0][q].size();w++){ // run over all columns
                findMargins(q, w);
                updateMyMargins(min_x,max_x,min_y,max_y);
            }
        }
    }else{ // CASE 2: We are plotting a single columns for one or a few files.
       // QMessageBox::information(this, "Information", QString("You are in case 4. One or a few files (%1 plotted files) and one column") .arg(currFilePlotNum) );
       // QMessageBox::information(this, "Information", QString("The file number is 0 and the column is %1") .arg(columnPlot) );

        findMargins(0, columnPlot-1); // To get a first values, find margins of file 0 and col 1.
        getMyMargins(min_x,max_x,min_y,max_y);  // Used only for initialization of these doubles.
        for(int q=0; q<currFilePlotNum;q++){ // run over plotted files
                findMargins(q, columnPlot-1);
                updateMyMargins(min_x,max_x,min_y,max_y);
                //qDebug() << QString("The range of file %1 is x_min= %2, x_max = %3, y_min =%4, y_max=%5") .arg(q) .arg(min_x) .arg(max_x) .arg(min_y) .arg(max_y);
        }
    }

    // We record the output in these global variables.
    Plot_margins.clear();
    Plot_margins.resize(4);
    Plot_margins[0] = min_x;
    Plot_margins[1] = max_x;
    Plot_margins[2] = min_y;
    Plot_margins[3] = max_y;
}

void MainWindow::rescale_rob(){  // rescale axis
    double margin = 10; // percentage of margin
    double mean_x, mean_y, xlow, xhigh, ylow, yhigh;
    double min_abs_x_q, max_abs_x_q, min_abs_y_q, max_abs_y_q;

    findGlobalMargins();
    getMyMargins(min_abs_x_q,max_abs_x_q,min_abs_y_q,max_abs_y_q);

   //

/*
    if(columnPlot>0){ // If we only plot one column

        scaleMargins(columnPlot);

        min_abs_x_q = Plot_margins[0];
        max_abs_x_q = Plot_margins[1];
        min_abs_y_q = Plot_margins[2];
        max_abs_y_q = Plot_margins[3];

    }else{ // If we plot multiple columns

        scaleMargins(2);

        min_abs_x_q = Plot_margins[0];
        max_abs_x_q = Plot_margins[1];
        min_abs_y_q = Plot_margins[2];
        max_abs_y_q = Plot_margins[3];

        for(int q = 3; q< AllData[0][0].size()+1;q++){ //We run over all columns of file i
            scaleMargins(q);
            min_abs_x = Plot_margins[0];
            max_abs_x = Plot_margins[1];
            min_abs_y = Plot_margins[2];
            max_abs_y = Plot_margins[3];

            if(min_abs_x<min_abs_x_q){
                min_abs_x_q = min_abs_x;
            }
            if(max_abs_x>max_abs_x_q){
                max_abs_x_q=max_abs_x;
            }
            if(min_abs_y<min_abs_y_q){
                min_abs_y_q=min_abs_y;
            }
            if(max_abs_y>max_abs_y_q){
                max_abs_y_q=max_abs_y;
            }
        }
    }*/

    mean_x = (max_abs_x_q-min_abs_x_q);
    mean_y = (max_abs_y_q-min_abs_y_q);
   //double xlow, xhigh, ylow, yhigh;
   if(log){    //If log scale is selected....
       xlow = min_abs_x_q - (margin/100)*mean_x;
       xhigh = max_abs_x_q + (margin/100)*mean_x;
       ylow = pow(10, log10(min_abs_y_q) - fabs((margin/100)*log10(mean_y)));
       yhigh = pow(10, log10(max_abs_y_q) + fabs((margin/100)*log10(mean_y)));
   }else{
       xlow = min_abs_x_q - (margin/100)*mean_x;
       xhigh = max_abs_x_q + (margin/100)*mean_x;
       ylow = min_abs_y_q - (margin/100)*mean_y;
       yhigh = max_abs_y_q + (margin/100)*mean_y;
   }

   //QMessageBox::information(this, "Information", QString("The range is x_min= %1, x_max = %2, y_min =%3, y_max=%4") .arg(min_abs_x_q) .arg(max_abs_x_q) .arg(min_abs_y_q) .arg(max_abs_y_q)  );
    //qDebug() << QString("The range is x_min= %1, x_max = %2, y_min =%3, y_max=%4") .arg(min_abs_x_q) .arg(max_abs_x_q) .arg(min_abs_y_q) .arg(max_abs_y_q);

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

void MainWindow::plotall(){
    ui->plot->clearGraphs();
    QStringList pieces;
    QString graphName, colName, testname, oneColName;
    QPen myPen;
    switch(flag1){ // One or all files to be plotted.
        case 0:{ //Only plot one file
            currFilePlotNum =1;
            if(columnPlot>0){ // Only plot one column of one file
                myPen.setWidth(2);
                ui->plot->addGraph();
                ui->plot->graph()->setPen(myPen);
                ui->plot->graph(0)->setData(AllData[0][0][0],AllData[0][0][columnPlot-1]);
                pieces = filenames.at(0).split("/");
                pieces = pieces.at(pieces.length()-1).split(".");
                if(ColumnMinSize>2){ // If there are multiple columns.
                    oneColName =QString(" (Col. %1)") .arg(columnPlot);
                    ui->plot->graph()->setName(pieces.at( pieces.length()-2)+oneColName);
                }else{
                    ui->plot->graph()->setName(pieces.at( pieces.length()-2));
                }
            }else{ // Plot all columns of one file
                pieces = filenames.at(0).split("/");
                pieces = pieces.at(pieces.length()-1).split(".");
                graphName = pieces.at( pieces.length()-2);
                for (int q =1; q< AllData[0][0].size(); q++){ //run over columns.
                     myPen.setColor(RGB(ColorFunc(q-1,0.0), ColorFunc(q-1,-0.666667), ColorFunc(q-1,0.666667)));
                     myPen.setWidth(2);
                    ui->plot->addGraph();
                    ui->plot->graph()->setPen(myPen);
                    ui->plot->graph(q-1)->setData(AllData[0][0][0],AllData[0][0][q]);
                    colName=QString(" (Col. %1)") .arg(q+1);
                    ui->plot->graph()->setName(graphName+colName);
                }
            }
            flag1 = 0;
            rescale_rob();
            ui->plot->replot();
        break;
        }
        case 1:{ //Plot all spectra
        currFilePlotNum=plotNum;
        //QMessageBox::information(this, "Info", QString("Gonna plot all files and cols...") );
            //for (int i = 0; i< filenames.size();i++){    // RUN OVER ALL FILES.

            for (int i = 0; i< plotNum;i++){    // RUN OVER ALL OPENED FILES.
               // QMessageBox::information(this, "Info", QString("Gonna plot file number i = %1") .arg(i) );
                if(columnPlot>0){ // Only plot one column of all files

                    ui->plot->addGraph();
                    myPen.setColor(RGB(ColorFunc(i,0.0), ColorFunc(i,-0.666667), ColorFunc(i,0.666667)));
                    myPen.setWidth(2);
                    ui->plot->graph()->setPen(myPen);

                    ui->plot->graph(i)->setData(AllData[0][i][0],AllData[0][i][columnPlot-1]);
                    pieces = filenames.at(i).split("/");
                    pieces = pieces.at(pieces.length()-1).split(".");
                    if(ColumnMinSize>2){ // If there are multiple columns.
                        oneColName =QString(" (Col. %1)") .arg(columnPlot);
                        ui->plot->graph()->setName(pieces.at( pieces.length()-2)+oneColName);
                    }else{
                        ui->plot->graph()->setName(pieces.at( pieces.length()-2));
                    }
                }else{
                    myPen.setColor(RGB(ColorFunc(i,0.0), ColorFunc(i,-0.666667), ColorFunc(i,0.666667)));
                    myPen.setWidth(2);

                    pieces = filenames.at(i).split("/");
                    pieces = pieces.at(pieces.length()-1).split(".");
                    graphName = pieces.at( pieces.length()-2);
                       // QMessageBox::information(this, "Info", QString("Gonna plot all columns of file number %1") .arg(i) );

                        for (int q =1; q< AllData[0][i].size(); q++){   // Run over columns of file i
                            ui->plot->addGraph();
                            ui->plot->graph()->setPen(myPen);
                            ui->plot->graph(q-1+i*(AllData[0][0].size()-1))->setData(AllData[0][i][0],AllData[0][i][q]); // This is quite important. Graphs are sequentially named from 0 to n in steps of x, where x is the number of cols of each file.
                            colName=QString(" (Col. %1)") .arg(q+1);
                            ui->plot->graph()->setName(graphName+colName);
                           // QMessageBox::information(this, "Information", QString("Adding graphs to plot multiple columns of multiple files.") );
                        }
                }
                 ui->plot->replot();
            }
            //QMessageBox::information(this, "Information", QString("All plots are set, now it is time to rescale.") );
            flag1 = 1;
            rescale_rob();
            ui->plot->replot();
            // ui->plot->update(); // I don't know what update does.
        break;
        }
    }    
}

//*********GRAPH:UNDO FUNCTIONS**********

void MainWindow::Backup(){
    for(int m=(undo-1); m>=0;m--){                                                 // run over backups it was 3!!!!
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
    // first we check empty value
    int empty=undo+1;
    for (int q=0;q<(undo+1);q++){
        if(AllData[q].empty()){
            empty=q;
            break;
        }
    }

    // We move all data from [m+1] to [m]
    for(int m=0; m<undo;m++){ //IT WAS 4?!?!?!?!
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

    //Finally we clear the last undone action
    AllData[empty-1].clear();
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
        if(AllActions[AllActions.size()-1][0].toInt()==1){ //Specific column needs to be plotted.
            if(AllActions.size()==1){   //We want to recover the previous plotted column because it has been modified by certain actions.
                    columnPlot = 2; //Default value
            }else{
                    columnPlot = AllActions[AllActions.size()-1][2].toInt();
            }
            plotall();
        // Creates new column. Operating with reference column... Duplicate, Average, Add, Subtract, Multiply and divide:
        }else if (AllActions[AllActions.size()-1][0].toInt()==22 || AllActions[AllActions.size()-1][0].toInt()==35){ //Specific column needs to be plotted.
            columnPlot = AllActions[AllActions.size()-1][1].toInt();
            ColumnMinSize--;
            GetBackupFunc();
            plotall();
        }else if(AllActions[AllActions.size()-1][0].toInt()==4 || AllActions[AllActions.size()-1][0].toInt()==19 || AllActions[AllActions.size()-1][0].toInt()==5|| AllActions[AllActions.size()-1][0].toInt()==6){
            columnPlot = AllActions[AllActions.size()-1][1].toInt();
            //qDebug() << QString ("ColumnMinSize was %1") .arg(ColumnMinSize);
            ColumnMinSize=ColumnMinSize-AllActions[AllActions.size()-1][3].toInt(); //the number of columns generated is stored in AllActions.
            //qDebug() << QString ("ColumnMinSize is %1 because generated cols was %2") .arg(ColumnMinSize) .arg(AllActions[AllActions.size()-1][3].toInt());
            GetBackupFunc();
            plotall();
        }else if(AllActions[AllActions.size()-1][0].toInt()==17){ // If we undo a stacking option.
            VerticalShift=RememberInv[RememberInv.size()-1][1];
            RememberInv.resize(RememberInv.size()-1);
            GetBackupFunc();
            // plotall();
        }else if (AllActions[AllActions.size()-1][0].toInt()==21){ // KKA number of columns is kept constant.
            GetBackupFunc();                            //This must come BEFORE columnPlot and plotall cuz we need to recover all deleted columns before setting its previous number.
            columnPlot = AllActions[AllActions.size()-1][2].toInt();
            plotall();
        }else if (AllActions[AllActions.size()-1][0].toInt()==7){ // Delete Column
            GetBackupFunc();                            //This must come BEFORE columnPlot and plotall cuz we need to recover all deleted columns before setting its previous number.
            columnPlot = AllActions[AllActions.size()-1][2].toInt();
            ColumnMinSize++;
            plotall();
        }else if (AllActions[AllActions.size()-1][0].toInt()==36 || AllActions[AllActions.size()-1][0].toInt()==37 ){ // Delete Column
            GetBackupFunc();                            //This must come BEFORE columnPlot and plotall cuz we need to recover all deleted columns before setting its previous number.
            ColumnMinSize=ColumnMinSize-2;
            plotall();
        }else if (AllActions[AllActions.size()-1][0].toInt()==34){ // Delete all Columns except one.
            GetBackupFunc();                            //This must come BEFORE columnPlot and plotall cuz we need to recover all deleted columns before setting its previous number.
            columnPlot = AllActions[AllActions.size()-1][2].toInt();
            ColumnMinSize=AllActions[AllActions.size()-1][3].toInt()+2;
            plotall();
        }else{                              //Same column as before action is plotted.
            GetBackupFunc();
            plotall();
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
    AllData.resize(undo+1); // so that it can be undone "undo" number of times.
    AllActions.clear();
}

//*********FORMAT FUNCTIONS**********

void MainWindow::startValue(QString datafile, QString outputdatafile, double start){
    QVector<QVector<QVector<QVector<double>>>> TempArr;
    QVector<QVector<double>> Answer;
    TempArr.resize(1);
    int count = 0;

    if (getOneData(datafile, TempArr)==0){ // If there's no error. BTW. this should create TempArr.
        Answer.resize(TempArr[0][0].size());
        for(int q =0; q<Answer.size();q++){
            Answer[q].resize(TempArr[0][0][q].size());
        }

        double currValue = start;
        for(int e0 =0; e0<Answer[0].size(); e0++ ){ // We run over rows

            for(int e1 =1; e1<Answer[0].size(); e1++ ){ // We run over rows
                if( ((currValue<TempArr[0][0][0][e1])&&(currValue>TempArr[0][0][0][e1-1]))||((currValue>TempArr[0][0][0][e1])&&(currValue<TempArr[0][0][0][e1-1])) ){
                    Answer[0][e0] = currValue;

                    for(int r = 1; r< Answer.size(); r++){  // We run over columns
                        Answer[r][e0] = (Answer[0][e0]-TempArr[0][0][0][e1-1])*(TempArr[0][0][r][e1]-TempArr[0][0][r][e1-1])/(TempArr[0][0][0][e1]-TempArr[0][0][0][e1-1]) + TempArr[0][0][r][e1-1];
                    }
                currValue = currValue + (TempArr[0][0][0][e1]-TempArr[0][0][0][e1-1]) ;
                break;
                }
            }
            count ++;
            // If we try to get data offlimits, stop acquiring data.
            if( !(((currValue<TempArr[0][0][0][0])&&(currValue>TempArr[0][0][0][TempArr[0][0][0].size()-1]))||((currValue>TempArr[0][0][0][0])&&(currValue<TempArr[0][0][0][TempArr[0][0][0].size()-1]))) ){
                break;
            }
        }


    }else{
        qDebug() << "Error in startValue function, couldnt get one data.";
        return;
    }

    // We open the temp output file where good lines are copied.
    QByteArray be = outputdatafile.toLocal8Bit(); //convert fitxer into char c_str2
    const char *c_str3 = be.data();
    ofstream output(c_str3, ios::out);

    for(int w = 0; w< count ;w++){     // run over acquired rows
        for(int q = 0; q< Answer.size();q++){            // run over columns
           output << std::scientific << Answer[q][w];
           if(!(q==(Answer.size()-1))){
               output << "\t";
           }
        }
        if(!(w==(count-1))){
            output << "\n";
        }
    }
    output.close();
}

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
    error++; // To dismiss the warning that error is unused.

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
        //QMessageBox::information(this, "Information", QString("So far so good.") );

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
       // QMessageBox::information(this, "Information", QString("So far so good.") );

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

    int p = 0, w =0;

    while (std::getline(obre, line)) {                  //Run over rows
            if(p>=lines){   // Only copies lines larger than the value introduced by user.
                if(w==0){                       // In order to prevent that an extra empty line is left at the end of the file.
                    temp << line;
                }else{
                    temp << endl << line;
                }
                w++;
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

    int p = 0, w=0;
    int numOfRows = RawLineNumbers(datafile);

    while (std::getline(obre, line)) {                  //Run over rows
            if(p<=(numOfRows-lines)){   // Only copies lines larger than the value introduced by user.
                if(w==0){                       // In order to prevent that an extra empty line is left at the end of the file.
                    temp << line;
                }else{
                    temp << endl << line;
                }
                w++;
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
        int p = 1, w =1;
        int deletedRows=0;

        while (std::getline(obre, line)) {                  //Run over rows

                if( validate_row(line) || (p<=caption) ){   // Respects validated lines or caption lines.
                    if(w==1){               // In order to prevent that an extra empty line is left at the end of the file.
                        temp << line;
                    }else{
                        temp << endl << line;
                    }
                    w++;
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

bool MainWindow::ActionsPerformed(){
    int action;
    if(AllActions.isEmpty()){
        return false;
    }else{
        if(AllActions.size()<1){
            return false;
        }
        for(int q = 0; q < AllActions.size();q++){
            action = AllActions[q][0].toInt();
            if(action != 1){
                return true;
            }
        }
        return false;
    }
}

// ******************************************************************************************************************
// ***************************************************** ACTIONS ****************************************************
// ******************************************************************************************************************

// *********************************************** File actions ***********************************************

void MainWindow::on_actionBackup_triggered(){
    // 1- We check if files have been opened or imported.
    if(filenames.isEmpty()){
        if(importnames.isEmpty()){
            QMessageBox::warning(this, "Warning", QString("Please, first import or open new files to proceed with this action.") );
            return;
        }else{
            if(checkBackupExists(importnames[0])){
                int reply = QMessageBox::question(this, "Attention!", "A backup folder has been detected. If you proceed its contents will be overwritten. Are you sure you want to proceed?",
                                              QMessageBox::Yes|QMessageBox::No);
                if (reply == QMessageBox::No) {
                    return;
                }
            }
            if(AllFormatActions.isEmpty()){
             if(AllFormatActions.size()>0){
                QMessageBox::information(this, "Information", QString("Some format actions have been dected. Please note that backup will not save current changes.") );
             }
            }
        }
    }else{
        if(checkBackupExists(filenames[0])){
            int reply = QMessageBox::question(this, "Attention!", "A backup folder has been detected. If you proceed its contents will be overwritten. Are you sure you want to proceed?",
                                          QMessageBox::Yes|QMessageBox::No);
            if (reply == QMessageBox::No) {
                return;
            }
        }
        if(AllActions.isEmpty()){
         if(AllActions.size()>0){
            QMessageBox::information(this, "Information", QString("Some actions have been dected. Please note that backup will not save current changes.") );
         }
        }
    }


    // 1- We create the backup folder.
    QStringList NameOfFiles;
    if(!filenames.isEmpty()){
        for(int q =0; q< filenames.size(); q++){
            NameOfFiles.append(filenames[q]);
        }
    }else{
        for(int q =0; q< importnames.size(); q++){
            NameOfFiles.append(importnames[q]);
        }
    }

    QFileInfo inputprelputinfo(NameOfFiles[0]);
    QString temp = inputprelputinfo.path() + "/backup/" + inputprelputinfo.completeBaseName() + "." + inputprelputinfo.suffix();
    QFileInfo backupfile(temp);

    QDir dir(backupfile.path());
    dir.removeRecursively(); //If backup file exist we delete it.

    while(!QDir(backupfile.path()).exists()){ //If the output folder doesnt exist we create it.     //IT WAS IF!!!!
        dir.mkpath(backupfile.path());
    }

    // 2- We copy all opened files into the backup folder
    bool error = false;
    for(int q =0; q< NameOfFiles.size(); q++){
        QFileInfo filenameInfo(NameOfFiles[q]);
        temp = filenameInfo.path() + "/backup/" + filenameInfo.completeBaseName() + "." + filenameInfo.suffix();
        if(!QFile::copy(NameOfFiles[q],temp)){        // Copy files and check if there is any error during the process.
            error = true;
        }
    }

    // 3- We show status bar info
    if(error){
        ui->statusbar->showMessage(QString("Some errors occured and file(s) could not be copied in the '%2' folder. Close all docs from the backup folder and try again.") .arg(backupfile.path()) ,10000);
    }else{
        ui->statusbar->showMessage(QString("%1 file(s) have been copied in the '%2' folder") .arg(NameOfFiles.size()) .arg(backupfile.path()),10000);
    }
}

void MainWindow::on_actionExport_data_triggered(){
    if(importnames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first import new files to proceed with this action.") );
        return;
    }else if(AllFormatActions.isEmpty()){
        ui->statusbar->showMessage("Data is not exported because no format actions were performed.",10000);
        return;
    }

    int numberImported =importnames.size();
    SaveAFA(true);
    QFileInfo inputputinfo(importnames[0]);
    QString check = inputputinfo.path() + Tempfolder; //We want to check if TempPreview exist, and if it does delete it.
    QDir dir(check);
    dir.removeRecursively();
    AllFormatActions.clear();
    prevExt.clear();
    importnames.clear(); // This is optional, and might be annoying for the user to import the files each time after export, but I think it will prevent errors, specially with the UNDO option.

    ui->statusbar->showMessage(QString("%1 file(s) have been exported.") .arg(numberImported),10000);
}

void MainWindow::on_actionImport_data_triggered(){
    QStringList tempImportNames;

    tempImportNames =QFileDialog::getOpenFileNames(
                this,
                tr("Open File"),
                "C://",
                "All files (*.*);;Ascii Files (*.asc)");
    if(tempImportNames.isEmpty()){
        ui->statusbar->showMessage("No data was imported. No valid file name or directory was selected.",10000);
        return;
    }else{
       importnames= tempImportNames;
    }

    if (importnames.size()>0){
        if(importnames.size()==1){
            ui->statusbar->showMessage("One file has been succesfully imported. Operate with this file in the Format menu.",10000);
        }else{
            ui->statusbar->showMessage(QString("%1 files have been succesfully imported. Operate with these files in the Format menu.") .arg(importnames.size()),10000);
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
    newFileDirectory.clear();
    newFileDirectory = QFileDialog::getSaveFileName(this, tr("Set name for the new file"), "C://", "Text file (*.txt);;Ascii File (*.asc)");
    if(newFileDirectory.isEmpty() || newFileDirectory.isNull()){
        ui->statusbar->showMessage("No file(s) have been saved.",10000);
        return;
    }
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
    SaveAll(filenames, newFileNames);

    for(int i =0; i<filenames.size(); i++){             // Run over files.
        filenames[i]=newFileNames[i];
    }

    newFileNames.clear();
    AllActions.clear();
    VerticalShift=0; //reset some parameters in case they were manipulated.
    ui->statusbar->showMessage("File(s) have been saved.",10000);
}

void MainWindow::on_actionExport_column_to_single_file_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }else if(ColumnMinSize<=2){
        QMessageBox::warning(this, "Warning", QString("To perform this operation, selected datafiles should have more than 2 columns.") );
        return;
    }else if(AllActions.size()>0){
        QMessageBox::warning(this, "Warning!", QString("Before proceeding, first save all changes (Ctrl+S)."));
        return;
    }

    int columnExport;
    if(!getIntNum(columnExport, 2, ColumnMinSize, QString("Current datafiles has %1 columns. \nTo export a single column into a new file introduce column number (from 2 to %1) to be plotted and operated:") .arg(ColumnMinSize))){
        ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    //int columnExport = QInputDialog::getInt(this, "Input",QString("Current datafiles has %1 columns. \nTo export a single column into a new file introduce column number (from 2 to %1) to be plotted and operated:") .arg(ColumnMinSize),0,2,ColumnMinSize);

    //ACTION takes place here.
    newFileDirectory.clear();
    newFileDirectory = QFileDialog::getSaveFileName(this, tr("Set name for the new file"), "C://", "Text file (*.txt);;Ascii File (*.asc)");
    if(newFileDirectory.isEmpty() || newFileDirectory.isNull()){
        ui->statusbar->showMessage("No action was performed. No valid file name or directory was introduced.",10000);
        return;
    }

    QVector<QVector<double>> tempVec, tempVecLarge;
    int RowNumsAtI;
    tempVec.resize(1); //First column x.
    int error=0;
    for(int i =0; i<filenames.size(); i++){                         // Run over files.
        error += getOneSingleData(filenames.at(i), tempVecLarge);
        tempVec.resize(tempVec.size()+1);
        //RowNumsAtI = RowNums(filenames.at(i));                      // To save computational time.
        RowNumsAtI = tempVecLarge[0].size();                      // previous commented line reported one value less.
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
    QStringList tempFilenames;

    tempFilenames =QFileDialog::getOpenFileNames(
                this,
                tr("Open File"),
                "C://",
                "All files (*.*);;Ascii Files (*.asc)");
    //We plot the first file
    if(tempFilenames.isEmpty()){
        ui->statusbar->showMessage("No data files were opened.",10000);
        return;
    }else{
        filenames=tempFilenames;
    }

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
            rescale_rob();
            ui->plot->replot();
            ui->plot->update();
        }

        plotall();

        ui->plot->legend->setVisible(true); // We show legend by default.
        ui->plot->replot();

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
        ui->plot->clearGraphs();
        ui->plot->replot();
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
        //qDebug() << QString("ColumnMinSize before save is %1") .arg(ColumnMinSize);
        SaveAll(filenames, filenames);
        AllActions.clear();
        VerticalShift=0; //reset some parameters in case they were manipulated.
        ui->statusbar->showMessage("All Files have been saved.",10000);
        //qDebug() << QString("ColumnMinSize after save is %1") .arg(ColumnMinSize);
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
    newFileDirectory.clear();
    newFileDirectory = QFileDialog::getExistingDirectory(this, tr("Set directory"), "C://");
    if(newFileDirectory.isEmpty() || newFileDirectory.isNull()){
        ui->statusbar->showMessage("No file(s) have been saved.",10000);
        return;
    }
    QStringList pieces;
    for(int i =0; i<filenames.size(); i++){
        pieces = filenames.at(i).split("/");
        newFileNames += newFileDirectory; //newFileNames is a QStringList
        newFileNames[i].append(QString("%1%2").arg("/").arg(pieces.at( pieces.length()-1)));
    }

    SaveAll(filenames, newFileNames);

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
        QMessageBox::information(this,"Info", QString("To combine pairs of datafiles new files need to be opened first, then files to be combined must be selected in 'Columns and files'->'Add reference file(s)'. Opened files will be merged with reference files in alphabetical order.") );
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
        plotall();
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

    if(fileName.isEmpty() || fileName.isNull()){
        ui->statusbar->showMessage(QString("No action was performed. Invalid directory."),10000);
        return;
    }

    SaveData(fileName, Output);
    TemporalArr1.clear();
    Output.clear();

        ui->statusbar->showMessage(QString("All data has been merged into a single file."),10000);
}

void MainWindow::on_actionColumns_to_files_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }else if(ColumnMinSize<=2){
        QMessageBox::warning(this, "Warning", QString("To perform this operation, selected datafiles should have more than 2 columns.") );
        return;
    }else if(AllActions.size()>0){
        QMessageBox::warning(this, "Warning!", QString("Before proceeding, first save all changes (Ctrl+S)."));
        return;
    }

    int columnExport;
    if(!getIntNum(columnExport, 0, ColumnMinSize, QString("Current datafiles has %1 columns. \nTo export all columns into new files introduce 0. \nTo export a single column into new files introduce column number (from 2 to %1) to be plotted and operated:") .arg(ColumnMinSize))){
        ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    //int columnExport = QInputDialog::getInt(this, "Input",QString("Current datafiles has %1 columns. \nTo export all columns into new files introduce 0. \nTo export a single column into new files introduce column number (from 2 to %1) to be plotted and operated:") .arg(ColumnMinSize),0,0,ColumnMinSize);

    if (columnExport ==1){
        QMessageBox::warning(this, "Warning!", QString("Please, either select 0 to export all columns into new files, or select a column number between 2 and %1") .arg(ColumnMinSize) );
        return;
    }

    //ACTION takes place here.
    newFileDirectory.clear();
    newFileDirectory = QFileDialog::getExistingDirectory(this, tr("Set directory to save new files"), "C://");

    if(newFileDirectory.isEmpty() || newFileDirectory.isNull()){
        ui->statusbar->showMessage(QString("No action was performed. Invalid directory."),10000);
        return;
    }

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
            //RowNumsAtI = RowNums(filenames.at(i));                      // To save computational time.
            RowNumsAtI = tempVecLarge[0].size();                      // previous commented line reported one value less.
            tempVec[0].resize(RowNumsAtI);
            tempVec[1].resize(RowNumsAtI);
            //qDebug() << QString("The last item of tempVecLarge is X= %1") .arg(tempVecLarge[0][tempVecLarge[0].size()-1]);
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

void MainWindow::on_actionSet_spectra_to_zero_triggered(){
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.

    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        setPlotToZero(AllData[0][q],columnPlot);
    }

    // 4- We plot all.
    plotall();

    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 33;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]="33";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);

    ui->statusbar->showMessage(QString("All plots have been set to zero."),10000);
}

void MainWindow::on_actionCallibrate_data_triggered(){
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    double result1, result2;
    if(!getDoubleNum(result1, "First introduce reference value:\n(i.e. value used as a callibrant from the literature)")){
        return;
    }
    if(!getDoubleNum(result2, "Introduce measured value to be corrected:")){
        return;
    }
    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.


    double shift = result1 - result2;

    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        AddConstantX(AllData[0][q],shift);
    }

    // 4- We plot all.
    plotall();

    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);
    RememberInv[RememberInv.size()-1][0]=shift;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]="32";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(shift);

    ui->statusbar->showMessage(QString("x Axis has been calibrated."),10000);
}

void MainWindow::on_actionEnergy_units_triggered(){
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.

    // 2.5- We get input from user if needed.
    QStringList items;
    items << tr("eV") << tr("meV") << tr("cm-1") << tr("THz") << tr("nm") << tr("um") << tr("J");

    bool ok;
    QString item1 = QInputDialog::getItem(this, tr("Input"),tr("First select current unit of x Axis:"), items, 0, false, &ok);
    if (ok && !item1.isEmpty()){
        // QMessageBox::information(this, "Information", QString("You have selected %1 with index %2") .arg(item1) .arg(items.indexOf(item1)) );
    }else{
        return;
    }
    QString item2 = QInputDialog::getItem(this, tr("Input"),QString("Current unit is %1.\nSelect desired unit for x Axis:").arg(item1), items, 0, false, &ok);
    if (ok && !item2.isEmpty()){
       // QMessageBox::information(this, "Information", QString("You have selected %1 with index %2") .arg(item2) .arg(items.indexOf(item2)) );
    }else{
        return;
    }

    int previousUnit = items.indexOf(item1);
    int newUnit = items.indexOf(item2);

    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        UnitsEnergy(AllData[0][q], previousUnit, newUnit);
    }

    // 4- We plot all.
    plotall();

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="31";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(previousUnit);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(newUnit);

    ui->statusbar->showMessage(QString("Units of x Axis have been modified from %1 to %2") .arg(item1) .arg(item2), 10000);
}

void MainWindow::on_actionLinear_baseline_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
    for(int q=0; q<AllData[0].size();q++){
        LinearBaseline(AllData[0][q], columnPlot);
    }

    plotall();


    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 10;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]="10";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    VerticalShift=0; //We reset the vertical stacking

    ui->statusbar->showMessage(QString("A linear baseline has been subtracted"),10000);
}

void MainWindow::on_actionBall_like_baseline_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    double result;
    if(!getDoubleNum(result, QString("Set ball radii. Suggested value = %1. Please note that this action can take large computational time: Avoid using too large ball radii."). arg(abs(AllData[0][0][0][0]-AllData[0][0][0][AllData[0][0][0].size()-1])/2))){
        return;
    }

    Backup();

    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);

    for(int q=0; q<AllData[0].size();q++){
        BallBaseline(AllData[0][q], columnPlot, result);
    }
    plotall();
    ui->statusbar->showMessage(QString("A ball-like baseline has been subtracted"),10000);
    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="18";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][2]=QString("%2") .arg(result);
    RememberInv[RememberInv.size()-1][0]=result;
    VerticalShift=0; //We reset the vertical stacking
}

void MainWindow::on_actionTrim_triggered(){ // right now this function will trim ALL columns! Future versions should be able to trim just one column.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    double min,max;
    double AbsMin, AbsMax; //Absolute min and max amongst all spectra
    double minMax, maxMin; //min max is the lowest max value, and max min is the largest lowest value amongst all files.
    AbsMin = FindMinFile(filenames.at(0));
    AbsMax = FindMaxFile(filenames.at(0));
    minMax=AbsMin;
    maxMin=AbsMax;

    for(int q=1; q<filenames.size();q++){
        min = FindMinFile(filenames.at(q));
        max = FindMaxFile(filenames.at(q));
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
        if(!getDoubleNum(result1, QString("Introduce lowest value to trim the plot (plot starts at %1). Introduced value must be lower than %2.") .arg(AbsMin) .arg(maxMin))){
            return;
        }
        if(!(result1<maxMin)){
          QMessageBox::warning(this, "Warning!", QString("Introduced value must be lower than %1. Please try again.") .arg(maxMin) );
        }else{
            error =0;
        }
    }

    error=1;
    while(error){
        if(!getDoubleNum(result2, QString("Introduce largest value to trim the plot (plot ends at %1). Introduced value must be larger than %2.") .arg(AbsMax) .arg(minMax))){
            return;
        }
        if(!(result2>minMax)){
          QMessageBox::warning(this, "Warning!", QString("Introduced value must be larger than %1. Please try again.") .arg(minMax) );
        }else{
            error =0;
        }
    }

    Backup();

    for(int p=0; p<AllData[0].size();p++){
        Trim(AllData[0][p], result1, result2);
    }

    plotall();
    ui->statusbar->showMessage(QString("All plots have been trimed from %1 to %2. This action affected all columns.").arg(result1).arg(result2),10000);
    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(2);

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="9";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(result1);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(result2);
    RememberInv[RememberInv.size()-1][0]=result1;
    RememberInv[RememberInv.size()-1][1]=result2;
}

void MainWindow::on_actionClean_spikes_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    int spikes = 0;
    int recsigma=0; //recommended sigma and points.
    double sigmas;
    int wpow2=0;
    int points;
    if(!getIntNum(points, 3, 2147483647, QString("Tolerance parameters: Introduce number of statistical points (recommended %1, minimum 5)") .arg(AllData[0][0][0].size()/20))){
        ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    //int points = QInputDialog::getInt(this, "Input", QString("Tolerance parameters: Introduce number of statistical points (recommended %1, minimum 5)") .arg(AllData[0][0][0].size()/20),0,-2147483647,2147483647,1);
    int columnInspect;

    if(columnPlot==0){
        columnInspect =2;
    }else{
        columnInspect=columnPlot;
    }

    for(int w=1;w<10;w++){ //Trying to find a recommended value for sigma.
        wpow2=w*w*w;
        spikes = spikinator(AllData[0][0], points, columnInspect, wpow2, 0);
        if((spikes>0) && (100*spikes/AllData[0][0][columnInspect-1].size()<10.0)){ //If some spikes were detected but represent less than 15% of the data.
            recsigma=wpow2;
            //QMessageBox::information(this, "Information", QString("at sigma %1, %2 spikes should be corrected") .arg(wpow2) .arg(spikes));
        }
    }

    if(recsigma==0){
        if(!getDoubleNum(sigmas, "Tolerance parameters: Introduce number sigma tolerance (recommended 8)")){
            return;
        }
    }else{
        if(!getDoubleNum(sigmas, QString("Tolerance parameters: Introduce number sigma tolerance (recommended %1 or lower)") .arg(recsigma))){
            return;
        }
    }
   spikes = 0;

   Backup();

    for (int q=0; q<AllData[0].size();q++){             //run over files
        spikes = spikes + spikinator(AllData[0][q], points, columnPlot, sigmas, 1);
    }
    plotall();

    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);
    RememberInv[RememberInv.size()-1][0]=sigmas;

    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 16;

    AllActions.resize(AllActions.size()+1);
    AllActions[AllActions.size()-1].resize(4);
    AllActions[AllActions.size()-1][0]="16";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(points);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][3]=QString("%1") .arg(sigmas);

    ui->statusbar->showMessage(QString("%1 spikes have been corrected per file") .arg(spikes/AllData[0].size()),10000);
}

void MainWindow::on_actionTo_first_column_Max_triggered(){
    //1- Warning messages. Check everything is set and ready to perform the action.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }else if(ColumnMinSize<3){
     QMessageBox::warning(this, "Warning", QString("To proceed with this action all datafiles must contain at least 3 columns (i.e. more than one 'y' Column).") );
     return;
    }
    // 2- We make backup in order to be able to undo actions if needed.
    Backup();

    // 3- We perform the action for all datafiles.
    for (int q=0; q<AllData[0].size();q++){             //run over files
        MultConstyMaxCol1(AllData[0][q]);
    }

    // 4- We plot the results.
    plotall();

    // 5- We save all needed values to perform such action and set action value in the vector AllActions, remind to include a description of the action in mainwindow.h.
    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(1);
    AllActions[AllActions.size()-1][0]="43";

    //6- We show a status bar message to the user to confirm that action has been performed.
    ui->statusbar->showMessage(QString("All columns have been normalized to the maximum value of the first column"),10000);
}

void MainWindow::on_actionTo_first_column_Area_triggered(){
    //1- Warning messages. Check everything is set and ready to perform the action.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }else if(ColumnMinSize<3){
     QMessageBox::warning(this, "Warning", QString("To proceed with this action all datafiles must contain at least 3 columns (i.e. more than one 'y' Column).") );
     return;
    }

    // 2- We make backup in order to be able to undo actions if needed.
    Backup();

    // 3- We perform the action for all datafiles.
    for (int q=0; q<AllData[0].size();q++){             //run over files
        MultConstyAreaCol1(AllData[0][q]);
    }

    // 4- We plot the results.
    plotall();

    // 4- We save all needed values to perform such action and set action value in the vector AllActions, remind to include a description of the action in mainwindow.h.
    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(1);
    AllActions[AllActions.size()-1][0]="42";

    //5- We show a status bar message to the user to confirm that action has been performed.
    ui->statusbar->showMessage(QString("All columns have been normalized to the area of the first column"),10000);
}

void MainWindow::on_actionTo_one_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();

//    double num, inv;
    for (int q=0; q<AllData[0].size();q++){             //run over files
        MultConsty1(AllData[0][q], columnPlot);
    }

    plotall();

    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 11;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]="11";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);

    ui->statusbar->showMessage(QString("All plots have been normalized to one"),10000);
}

void MainWindow::on_actionTo_first_file_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    if(columnPlot==0){
        QMessageBox::warning(this, "Warning", QString("This action cannot be performed while 'All Columns' option is active.") );
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
    plotall();

    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 13;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]="13";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);

    ui->statusbar->showMessage(QString("All plots have been normalized. Area of first file was %1") .arg(num1st),10000);
}

void MainWindow::on_actionTo_one_2_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();
 //   double num, inv;
    for (int q=0; q<AllData[0].size();q++){
        MultConsty2(AllData[0][q], columnPlot);
    }
    plotall();

    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 12;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]="12";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);

    ui->statusbar->showMessage(QString("All plots have been normalized to one"),10000);
}

void MainWindow::on_actionTo_first_file_2_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    if(columnPlot==0){
        QMessageBox::warning(this, "Warning", QString("This action cannot be performed while 'All Columns' option is active.") );
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
    plotall();


    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 14;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]="14";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);

    ui->statusbar->showMessage(QString("All plots have been normalized to one."),10000);
}

// *********************************************** Files and columns *******************************************

void MainWindow::on_actionAdd_ref_file_triggered(){
    // 0- We check if we opened proper files, number of files, set the working column of ref files (referenceCol) and checked X dataranges.
    if(!checkOperateRef() || !checkRefColumn() || !checkProperRanges() ){
        return;
    }

    // 1- We make a backup in order to be able to undo the action if user needs to.
    Backup();

    // 2- We call the function to plot the results.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        AddReference(AllData[0][q], q, columnPlot, referenceCol);
    }
    plotall();

    // 3- We save the action so that it can be undone, or applied to the datafiles upon saving.

    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 38;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="38";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(referenceCol);

    // 4- We update the status bar to confirm the used that the action has been performed.
    ui->statusbar->showMessage(QString("Column %1 of reference file(s) has been added to all plots") .arg(referenceCol) ,10000);
    return;
}

void MainWindow::on_actionSubtract_ref_file_s_triggered(){
    // 0- We check if we opened proper files, number of files, set the working column of ref files (referenceCol) and checked X dataranges.
    if(!checkOperateRef() || !checkRefColumn() || !checkProperRanges() ){
        return;
    }

    // 1- We make a backup in order to be able to undo the action if user needs to.
    Backup();

    // 2- We call the function to plot the results.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        SubtractReference(AllData[0][q], q, columnPlot, referenceCol);
    }
    plotall();

    // 3- We save the action so that it can be undone, or applied to the datafiles upon saving.
    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 40;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="40";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(referenceCol);

    // 4- We update the status bar to confirm the used that the action has been performed.
    ui->statusbar->showMessage(QString("Column %1 of reference file(s) has been subtracted to all plots") .arg(referenceCol) ,10000);
    return;
}

void MainWindow::on_actionMultiply_ref_files_s_triggered(){
    // 0- We check if we opened proper files, number of files, set the working column of ref files (referenceCol) and checked X dataranges.
    if(!checkOperateRef() || !checkRefColumn() || !checkProperRanges() ){
        return;
    }

    // 1- We make a backup in order to be able to undo the action if user needs to.
    Backup();

    // 2- We call the function to plot the results.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        MultiplyReference(AllData[0][q], q, columnPlot, referenceCol);
    }
    plotall();

    // 3- We save the action so that it can be undone, or applied to the datafiles upon saving.
    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 39;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="39";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(referenceCol);

    // 4- We update the status bar to confirm the used that the action has been performed.
    ui->statusbar->showMessage(QString("Column %1 of reference file(s) has been multiplied to all plots") .arg(referenceCol) ,10000);
    return;
}

void MainWindow::on_actionDivide_by_ref_file_s_triggered(){
    // 0- We check if we opened proper files, number of files, set the working column of ref files (referenceCol) and checked X dataranges.
    if(!checkOperateRef() || !checkRefColumn() || !checkProperRanges() ){
        return;
    }

    // 1- We make a backup in order to be able to undo the action if user needs to.
    Backup();

    // 2- We call the function to plot the results.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        DivideReference(AllData[0][q], q, columnPlot, referenceCol);
    }
    plotall();

    // 3- We save the action so that it can be undone, or applied to the datafiles upon saving.
    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 41;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="41";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(referenceCol);

    // 4- We update the status bar to confirm the used that the action has been performed.
    ui->statusbar->showMessage(QString("Column %1 of reference file(s) has been divided to all plots") .arg(referenceCol) ,10000);
    return;
}

void MainWindow::on_actionAverageFiles_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;

    // 0- Before function is called it should be checked that multiple files have the same number of rows, starting and ending in the same X value.
    }
    if(filenames.size()<2){
        QMessageBox::warning(this, "Warning", QString("Multiple files need to be opened in order to proceed with this action.") );
        return;
    }

    bool rows = true, start = true, end = true;
    int rowfirstnum = RowNums(filenames[0]);
    double firstnum = getFirstX(filenames[0]);
    double lastnum = getLastX(filenames[0]);

    for(int q=1; q< filenames.size(); q++){
        if(rowfirstnum!=RowNums(filenames[q])){
            rows=false;
        }
        if(firstnum!=getFirstX(filenames[q])){
            start=false;
        }
        if(lastnum!=getLastX(filenames[q])){
            end=false;
        }
    }
    if(!rows){
        QMessageBox::warning(this, "Warning", QString("Files have a different number of rows. No action was performed.") );
        return;
    }
    if(!start){
        QMessageBox::warning(this, "Warning", QString("Col 1 (X) of files start with different values. No action was performed.") );
        return;
    }
    if(!end){
        QMessageBox::warning(this, "Warning", QString("Col 1 (X) of files end with different values. No action was performed.") );
        return;
    }

    // 1- We get the input parameters
    QString outputFilename;
    outputFilename = QInputDialog::getText(this, "Input", "Introduce the filename where result will be shown:\n(avoid introducing an existing file's name since it would be overwritten)\n(introduce filename without extention, e.g. 'result_sum_files')");
    if(outputFilename.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Text string is empty. No operation was performed.") );
        return;
    }

    int columnToAdd;
    if(ColumnMinSize>2){
        if(!getIntNum(columnToAdd, 2, ColumnMinSize, QString("Introduce column number to be averaged from 2 to %1:") .arg(ColumnMinSize))){
            ui->statusbar->showMessage("No action was performed.",10000);
            return;
        }
        //columnToAdd = QInputDialog::getInt(this, "Input",QString("Introduce column number to be averaged from 2 to %1:") .arg(ColumnMinSize), 0, 2, ColumnMinSize, 1);
    }else{
        columnToAdd = 2;
    }

    double toaverage = (double)filenames.size();
    // 2- ACTION takes place here
        addFiles(outputFilename, columnToAdd, toaverage);

    plotall();
    ui->statusbar->showMessage(QString("Col %1 of all files have been averaged. Result shown in file %2") .arg(AllData[0][0].size()) .arg(outputFilename),10000);
}

void MainWindow::on_actionAddMultFiles_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;

    // 0- Before function is called it should be checked that mulstiple files have the same number of rows, starting and ending in the same X value.
    }
    if(filenames.size()<2){
        QMessageBox::warning(this, "Warning", QString("Multiple files need to be opened in order to proceed with this action.") );
        return;
    }

    bool rows = true, start = true, end = true;
    int rowfirstnum = RowNums(filenames[0]);
    double firstnum = getFirstX(filenames[0]);
    double lastnum = getLastX(filenames[0]);

    for(int q=1; q< filenames.size(); q++){
        if(rowfirstnum!=RowNums(filenames[q])){
            rows=false;
        }
        if(firstnum!=getFirstX(filenames[q])){
            start=false;
        }
        if(lastnum!=getLastX(filenames[q])){
            end=false;
        }
    }
    if(!rows){
        QMessageBox::warning(this, "Warning", QString("Files have a different number of rows. No action was performed.") );
        return;
    }
    if(!start){
        QMessageBox::warning(this, "Warning", QString("Col 1 (X) of files start with different values. No action was performed.") );
        return;
    }
    if(!end){
        QMessageBox::warning(this, "Warning", QString("Col 1 (X) of files end with different values. No action was performed.") );
        return;
    }

    // 1- We get the input parameters
    QString outputFilename;
    outputFilename = QInputDialog::getText(this, "Input", "Introduce the filename where result will be shown:\n(avoid introducing an existing file's name since it would be overwritten)\n(introduce filename without extention, e.g. 'result_sum_files')");
    if(outputFilename.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Text string is empty. No operation was performed.") );
        return;
    }

    int columnToAdd;
    if(ColumnMinSize>2){
        if(!getIntNum(columnToAdd, 0, ColumnMinSize, QString("Introduce column number to be added from 2 to %1:") .arg(ColumnMinSize))){
            ui->statusbar->showMessage("No action was performed.",10000);
            return;
        }
        //columnToAdd = QInputDialog::getInt(this, "Input",QString("Introduce column number to be added from 2 to %1:") .arg(ColumnMinSize), 0, 2, ColumnMinSize, 1);
    }else{
        columnToAdd = 2;
    }

    // 2- ACTION takes place here
        addFiles(outputFilename, columnToAdd, 1.0);

    plotall();
    ui->statusbar->showMessage(QString("Col %1 of all files have been added. Result shown in file %2") .arg(AllData[0][0].size()) .arg(outputFilename),10000);
}

void MainWindow::on_actionAverage_all_columns_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    if(ColumnMinSize>2){               // If there are more than 2 columns in all files.
        Backup();// It is very important to include the backup BEFORE any data manipulation.
        int previouscolumnPlot = columnPlot;

        //ACTION takes place here
        for(int q=0; q<AllData[0].size();q++){  //Run over files.
            averageAllCols(AllData[0][q]);
        }
        if(columnPlot!=0){
            columnPlot = AllData[0][0].size();
        }
        ColumnMinSize++;
        plotall();
        ui->statusbar->showMessage(QString("Averaged columns have been added to the last column, number %1") .arg(AllData[0][0].size()),10000);

        AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
        AllActions[AllActions.size()-1].resize(2);
        AllActions[AllActions.size()-1][0]="35";
        AllActions[AllActions.size()-1][1]=QString("%1") .arg(previouscolumnPlot);
    }else{
        QMessageBox::warning(this, "Warning!", QString("Action was not performed. Actual file(s) don't have enough columns. Some files only have %1 columns.") .arg(ColumnMinSize));
    }
}

void MainWindow::on_actionDelete_all_columns_except_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    if(ColumnMinSize>2){               // If there are more than 2 columns in all files.
        int result;
        if(!getIntNum(result, 2, ColumnMinSize, QString("Set column number to be respected from 2 to %1:\n(All other columns -y Axis- will be deleted)") .arg(ColumnMinSize))){
            ui->statusbar->showMessage("No action was performed.",10000);
            return;
        }
        //int result = QInputDialog::getInt(this, "Input",QString("Set column number to be respected from 2 to %1:\n(All other columns -y Axis- will be deleted)") .arg(ColumnMinSize), 0, 2, ColumnMinSize, 1);

        Backup();// It is very important to include the backup BEFORE any data manipulation.

        previouscolPlot = columnPlot;
        columnPlot=2;

        //ACTION takes place here
        int DeletedCols =AllData[0][0].size();
        int temp =0;
        for(int q=0; q<AllData[0].size();q++){  //Run over files.
            temp = DeleteExceptCol(AllData[0][q], result);
            if (temp < DeletedCols){
                DeletedCols = temp;
            }
        }

        ColumnMinSize=2;

        plotall();
        ui->statusbar->showMessage(QString("All columns except Col. %1 have been deleted").arg(result),10000);

        AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
        AllActions[AllActions.size()-1].resize(4);
        AllActions[AllActions.size()-1][0]="34";
        AllActions[AllActions.size()-1][1]=QString("%1") .arg(result);
        AllActions[AllActions.size()-1][2]=QString("%1") .arg(previouscolPlot);
        AllActions[AllActions.size()-1][3]=QString("%1") .arg(DeletedCols);
    }else{
        QMessageBox::warning(this, "Warning!", QString("No columns can be deleted. Actual file(s) don't have enough columns. Some files only have %1 columns.") .arg(ColumnMinSize));
    }

}

void MainWindow::on_actionSet_Plot_Column_triggered(){          // Set plot column
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    if (filenames.size()<=0){   // I dont think this is needed.
        QMessageBox::information(
            this,
            tr("Warning"),
            tr("No valid data was opened") );
    }

    int previouscolPlot = columnPlot;

    if(ColumnMinSize>2){ // If files have multiple columns, the option to activated 'All Columns mode' will be offered.
        if(!getIntNum(columnPlot, 0, ColumnMinSize, QString("Current datafiles has %1 columns. Introduce column number (from 1 to %1) to be plotted and operated:\n(to activate 'All Columns mode' introduce 0)") .arg(ColumnMinSize))){
                ui->statusbar->showMessage("No action was performed.",10000);
            return;
        }

        //columnPlot = QInputDialog::getInt(this, "Input",QString("Current datafiles has %1 columns. Introduce column number (from 1 to %1) to be plotted and operated:\n(to activate 'All Columns mode' introduce 0)") .arg(ColumnMinSize),0,0,ColumnMinSize);
    }else{
        if(!getIntNum(columnPlot, 1, ColumnMinSize, QString("Current datafiles has %1 columns. Introduce column number (from 1 to %1) to be plotted and operated:") .arg(ColumnMinSize))){
                ui->statusbar->showMessage("No action was performed.",10000);
            return;
        }
        //columnPlot = QInputDialog::getInt(this, "Input",QString("Current datafiles has %1 columns. Introduce column number (from 1 to %1) to be plotted and operated:") .arg(ColumnMinSize),0,1,ColumnMinSize);
    }

    if(columnPlot == 0){ // All columns are selected.
        //if(!AllActions.isEmpty()){
        if(ActionsPerformed()){
            QMessageBox::warning(this,"Warning!", QString("Actions have been detected. \nBefore switching to 'All Columns mode', all changes need to be saved (Ctrl+S) or discarded by opening files again (Ctrl+O).\nNo action was performed.") );
            columnPlot=previouscolPlot;  //set back to previous value.
            return;
        }else{
            //AllActions.clear();
            AllColumns = true;
            ui->statusbar->showMessage(QString("'All Columns mode' is activated. All columns have been plotted."),10000);

        }
    }else{  // One single column is selected.
        //if(AllColumns && AllActions.size()>0){
        if(AllColumns && ActionsPerformed()){
            QMessageBox::warning(this,"Warning!", QString("Actions have been detected. \nBefore switching off 'All Columns mode', all changes need to be saved or discarded by opening files again.\nNo action was performed.") );
            columnPlot=previouscolPlot;  //set back to previous value.
            return;
        }
        //AllActions.clear();
        AllColumns = false;
        ui->statusbar->showMessage(QString("Column %1 has been plotted.").arg(columnPlot),10000);
    }

    if(log){ // If log scale is selected.
        for (int i = 0; i< filenames.size();i++){
        double* min_y = std::min_element(AllData[0][i][columnPlot-1].begin(), AllData[0][i][columnPlot-1].end());
            if(*min_y<=0){
                QMessageBox::warning(this, "Warning!", QString("Some datafiles contain negative y values. Log scale cannot be represented. Select linear scale before attempting to select column number %1.") .arg(columnPlot) );
                columnPlot= previouscolPlot;
                return;
            }
        }
    }

    plotall();

    AllActions.resize(AllActions.size()+1);
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="1";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(previouscolPlot);

//    ui->statusbar->showMessage(QString("Column %1 has been selected and plotted").arg(columnPlot),10000);
}

void MainWindow::on_actionSet_x_column_triggered(){             // Rearange columns
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    Backup();

    int result1, result2;
    if(!getIntNum(result1, 1, ColumnMinSize, QString("Introduce first column number to be swapped from 1 to %1") .arg(ColumnMinSize))){
            ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    if(!getIntNum(result2, 1, ColumnMinSize, QString("Introduce second column number to be swapped from 1 to %1") .arg(ColumnMinSize))){
            ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }

    //int result1 = QInputDialog::getInt(this, "Input",QString("Introduce first column number to be swapped from 1 to %1") .arg(ColumnMinSize),1,1,ColumnMinSize);
    //int result2 = QInputDialog::getInt(this, "Input",QString("Introduce second column number to be swapped from 1 to %1") .arg(ColumnMinSize),1,1,ColumnMinSize);

    for(int q=0; q<AllData[0].size(); q++){
        SwapCols(AllData[0][q], result1, result2);
    }
    plotall();
    ui->statusbar->showMessage(QString("Columns %1 and %2 have been swapped").arg(result1) .arg(result2),10000);

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="8";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(result1);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(result2);
}

void MainWindow::on_actionDelete_column_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    if(ColumnMinSize>2){               // If there are more than 2 columns in all files.
        int result;
        if(!getIntNum(result, 2, ColumnMinSize, QString("Set column number to delete from 2 to %1") .arg(ColumnMinSize))){
                ui->statusbar->showMessage("No action was performed.",10000);
            return;
        }
        //int result = QInputDialog::getInt(this, "Input",QString("Set column number to delete from 2 to %1") .arg(ColumnMinSize), 0, 2, ColumnMinSize, 1);

        Backup();// It is very important to include the backup BEFORE any data manipulation.
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

        plotall();
        ui->statusbar->showMessage(QString("Column %1 has been deleted").arg(result),10000);

        AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
        AllActions[AllActions.size()-1].resize(3);
        AllActions[AllActions.size()-1][0]="7";
        AllActions[AllActions.size()-1][1]=QString("%1") .arg(result);
        AllActions[AllActions.size()-1][2]=QString("%1") .arg(previouscolPlot);
    }else{
        QMessageBox::warning(this, "Warning!", QString("No columns can be deleted. Actual file(s) don't have enough columns. Some files only have %1 columns.") .arg(ColumnMinSize));
    }
}

void MainWindow::on_actionDuplicate_Column_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    int result;
    if(!getIntNum(result, 1, ColumnMinSize, QString("Set column number to duplicate from 1 to %1") .arg(ColumnMinSize))){
            ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    //int result = QInputDialog::getInt(this, "Input",QString("Set column number to duplicate from 1 to %1") .arg(ColumnMinSize), 0, 1, ColumnMinSize, 1);

    Backup();// It is very important to include the backup BEFORE any data manipulation.
    int previouscolumnPlot = columnPlot;

    //ACTION takes place here
    for(int q=0; q<AllData[0].size();q++){      // Run over files.
        DuplicateCol(AllData[0][q], result);    // Duplicates a column to a new column at the end.
    }
    //We update the number of columns. How does delete does it without calling this function?
 //   findColRowNums(filenames); //Now we know if all files have or don't have the same amount of rows and columns, and their ranges. Info stored in FileRowNums[] and FileColNums[].

 //   QMessageBox::information(this, "Information", QString("Now there are %1 columns, FileColNums[1] is %2") .arg(AllData[0][0].size()).arg(FileColNums[1]) );

    if(ColumnMinSize==1){
        QMessageBox::warning(this, "Warning!", QString("Files have a different number of columns. The duplicated column will be added as a new column at the end of each datafile, hence its position won't be the same for all files.") );
    }


    if(columnPlot!=0){
        columnPlot = AllData[0][0].size();
    }
    ColumnMinSize++;
    plotall();
    ui->statusbar->showMessage(QString("Column %1 has duplicated and included as a new column at the end of each file. Col. %2 is ploted.") .arg(result) .arg(columnPlot),10000);

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="22";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(previouscolumnPlot);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(result);
}

void MainWindow::on_actionAdd_reference_column_triggered(){     // Set reference column
    if(!getIntNum(ReferenceColumn, 1, ColumnMinSize, QString("Introduce column number (from 1 to %1) to be used for operations with nominal data:") .arg(ColumnMinSize))){
            ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    //ReferenceColumn = QInputDialog::getInt(this, "Input",QString("Introduce column number (from 1 to %1) to be used for operations with nominal data:") .arg(ColumnMinSize),0,1,ColumnMinSize);
    if((ReferenceColumn<1) || (ReferenceColumn>ColumnMinSize)){ //This if is not really needed since the QInputDialog does not allow to take wrong numbers.
         QMessageBox::warning(this,"Warning!", QString("Column number introduced, %1, is not valid (please, introduce a number between 1 and %2)") .arg(ReferenceColumn) .arg(ColumnMinSize));
         ReferenceColumn=2; //set back to default value.
         return;
     }
        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(2);
        AllActions[AllActions.size()-1][0]="3";
        AllActions[AllActions.size()-1][1]=QString("%1") .arg(ReferenceColumn);
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
    QString messageQuestion;
    if(columnPlot==0){
        messageQuestion =  QString("Do you confirm that all columns will be added to Col. #%1?") .arg(ReferenceColumn);
    }else{
        messageQuestion = QString("Do you confirm that Col. #%1 will be added to Col. #%2?") .arg(columnPlot) .arg(ReferenceColumn);
    }
    reply = QMessageBox::question(this, "Question:", messageQuestion, QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {

        Backup();// It is very important to include the backup BEFORE any data manipulation.
        int previouscolumnPlot = columnPlot;
        int initialcolnums = AllData[0][0].size();
        int generatedColumns;

        //ACTION takes place here.
        for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
            AddColumns(AllData[0][q],columnPlot,ReferenceColumn);
        }

        if(columnPlot!=0){
            columnPlot = AllData[0][0].size();
            ColumnMinSize++;
            generatedColumns=1;
        }else if (columnPlot ==0){
            generatedColumns=AllData[0][0].size()-initialcolnums;
            ColumnMinSize=ColumnMinSize+generatedColumns;
        }
        plotall();

        ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
        ActionCol[ActionCol.size()-1] = 19;

        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(4);
        AllActions[AllActions.size()-1][0]="19";
        AllActions[AllActions.size()-1][1]=QString("%1") .arg(previouscolumnPlot);
        AllActions[AllActions.size()-1][2]=QString("%1") .arg(ReferenceColumn);
        AllActions[AllActions.size()-1][3]=QString("%1") .arg(generatedColumns);
        if(columnPlot!=0){
            ui->statusbar->showMessage(QString("Columns have been added. A new column %1 has been generated.") .arg(columnPlot) ,10000);
        }else{
            ui->statusbar->showMessage(QString("Columns have been added. %1 new columns have been generated.") .arg(generatedColumns) ,10000);
       }
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
    QString messageQuestion;
    if(columnPlot==0){
        messageQuestion =  QString("Do you confirm that all columns will be subtracted to Col. #%1?") .arg(ReferenceColumn);
    }else{
        messageQuestion = QString("Do you confirm that Col. #%1 will be subtracted by Col. #%2?") .arg(columnPlot) .arg(ReferenceColumn);
    }
    reply = QMessageBox::question(this, "Question:", messageQuestion, QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {

        Backup();// It is very important to include the backup BEFORE any data manipulation.
        int previouscolumnPlot = columnPlot;
        int initialcolnums = AllData[0][0].size();
        int generatedColumns;

        //ACTION takes place here
        for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
            SubtractColumns(AllData[0][q],columnPlot,ReferenceColumn);
        }

        if(columnPlot!=0){
            columnPlot = AllData[0][0].size();
            ColumnMinSize++;
            generatedColumns =1;
        }else if (columnPlot ==0){
            generatedColumns=AllData[0][0].size()-initialcolnums;
            ColumnMinSize=ColumnMinSize+generatedColumns;
        }
        plotall();

        ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
        ActionCol[ActionCol.size()-1] = 4;

        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(4);
        AllActions[AllActions.size()-1][0]="4";
        AllActions[AllActions.size()-1][1]=QString("%1") .arg(previouscolumnPlot);
        AllActions[AllActions.size()-1][2]=QString("%1") .arg(ReferenceColumn);
        AllActions[AllActions.size()-1][3]=QString("%1") .arg(generatedColumns);
        if(columnPlot!=0){
            ui->statusbar->showMessage(QString("Columns have been subtracted. A new column %1 has been generated.") .arg(columnPlot) ,10000);
        }else{
            ui->statusbar->showMessage(QString("Columns have been subtracted. %1 new columns have been generated.") .arg(generatedColumns) ,10000);
        }
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
    QString messageQuestion;
    if(columnPlot==0){
        messageQuestion =  QString("Do you confirm that all columns will be divided by Col. #%1?") .arg(ReferenceColumn);
    }else{
        messageQuestion = QString("Do you confirm that Col. #%1 will be divided by Col. #%2?") .arg(columnPlot) .arg(ReferenceColumn);
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Question:", messageQuestion, QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        Backup();// It is very important to include the backup BEFORE any data manipulation.
        int previouscolumnPlot = columnPlot;
        int initialcolnums = AllData[0][0].size();
        int generatedColumns;
        //ACTION takes place here
        for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
            DivideColumns(AllData[0][q],columnPlot,ReferenceColumn);
        }

        if(columnPlot!=0){
            columnPlot = AllData[0][0].size();
            ColumnMinSize++;
            generatedColumns =1;
        }else if (columnPlot ==0){
            generatedColumns=AllData[0][0].size()-initialcolnums;
            ColumnMinSize=ColumnMinSize+generatedColumns;
        }
        //QMessageBox::information(this, "Information for debug", QString("Operations made. Now it is time to plot all."));
        //qDebug() << QString("ColumnMinSize before plot is %1") .arg(ColumnMinSize);

        plotall();
        //qDebug() << QString("ColumnMinSize after plot is %1") .arg(ColumnMinSize);

        ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
        ActionCol[ActionCol.size()-1] = 5;

        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(4);
        AllActions[AllActions.size()-1][0]="5";
        AllActions[AllActions.size()-1][1]=QString("%1") .arg(previouscolumnPlot);
        AllActions[AllActions.size()-1][2]=QString("%1") .arg(ReferenceColumn);
        AllActions[AllActions.size()-1][3]=QString("%1") .arg(generatedColumns);
        if(columnPlot!=0){
            ui->statusbar->showMessage(QString("Columns have been divided. A new column %1 has been generated.") .arg(columnPlot) ,10000);
        }else{
            ui->statusbar->showMessage(QString("Columns have been divided. %1 new columns have been generated.") .arg(generatedColumns) ,10000);
        }
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
    QString messageQuestion;
    if(columnPlot==0){
        messageQuestion =  QString("Do you confirm that all columns will be multiplied to Col. #%1?") .arg(ReferenceColumn);
    }else{
        messageQuestion = QString("Do you confirm that Col. #%1 will be multiplied to Col. #%2?") .arg(columnPlot) .arg(ReferenceColumn);
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "Question:", messageQuestion, QMessageBox::Yes|QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        Backup();// It is very important to include the backup BEFORE any data manipulation.
        int previouscolumnPlot = columnPlot;
        int initialcolnums = AllData[0][0].size();
        int generatedColumns;

        // ACTION takes place here
        for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
            MultiplyColumns(AllData[0][q],columnPlot,ReferenceColumn);
            //QMessageBox::information(this, "Information for debug", QString("Running over files"));
        }

        if(columnPlot!=0){
            generatedColumns=1;
            columnPlot = AllData[0][0].size();
            ColumnMinSize++;
        }else if (columnPlot==0){
            generatedColumns=AllData[0][0].size()-initialcolnums;
            ColumnMinSize=ColumnMinSize+generatedColumns;
        }
        plotall();

        //QMessageBox::information(this, "Information for debug", QString("Column %1 is plotted, previous column was %2") .arg(columnPlot) .arg(previouscolumnPlot));

        ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
        ActionCol[ActionCol.size()-1] = 6;

        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(4);
        AllActions[AllActions.size()-1][0]="6";
        AllActions[AllActions.size()-1][1]=QString("%1") .arg(previouscolumnPlot);
        AllActions[AllActions.size()-1][2]=QString("%1") .arg(ReferenceColumn);
        AllActions[AllActions.size()-1][3]=QString("%1") .arg(generatedColumns);
        if(columnPlot!=0){
            ui->statusbar->showMessage(QString("Columns have been multiplied. A new column %1 has been generated.") .arg(columnPlot) ,10000);
        }else{
            ui->statusbar->showMessage(QString("Columns have been multiplied. %1 new columns have been generated.") .arg(generatedColumns) ,10000);
        }
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

void MainWindow::on_actionGenerate_code_triggered(){
    if(ActionsPerformed()){
        QString code = GenerateCode();
        QString tempOutput = QInputDialog::getText(this, tr("Code generated for all Actions"),tr("Please copy and keep the code below to apply all these actions for future files:\n(Tip #1: set cursor in the dialog below, then press Ctrl+A, Ctrl+C to copy code to clipboard.)\n(Tip #2: Save your codes in text files to perform repetitive actions on files located in different folders, or during different days.)"),QLineEdit::Normal,code);

    }else{
        QMessageBox::warning(this, "Warning", QString("No actions have been detected. No code was generated.\n(Note that the code should be generated right before saving.)") );
        return;
    }
}

void MainWindow::on_actionIntroduce_code_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }else if(ActionsPerformed()){
        QMessageBox::warning(this, "Warning", QString("Actions are detected. Save current changes before proceeding with current Action.") );
        return;
    }

    bool ok;
    QString code = QInputDialog::getText(this, tr("Introduce code"),tr("Introduce previously generated code to perform all those actions:"),QLineEdit::Normal,"", &ok);
    if(ok && !code.isEmpty()){
        // Here I will need to check if the code is valid, apply it to the opened files and then the user can save the changes if he/she likes them.

        ReadCode(code);
        plotall();
        PerformCodeActions();
        CheckExectuedCode(code);
        plotall();

        ui->statusbar->showMessage(QString("Code has been read and executed. Check that all changes are appropiate before saving.") ,10000);

    }else{
        ui->statusbar->showMessage(QString("No code was introduced. No action was performed.") ,10000);
    }
}

void MainWindow::on_actionR_Theta_X_Y_triggered(){
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    if(ColumnMinSize < 3){
        QMessageBox::warning(this, "Warning", QString("Datafiles don't have enough columns. A minimum of 3 columns is required.") );
        return;
    }

    int Rcol, Tcol;
    if(!getIntNum(Rcol, 2, ColumnMinSize, QString("Set R column number from 2 to %1") .arg(ColumnMinSize))){
            ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    if(!getIntNum(Tcol, 2, ColumnMinSize, QString("Set Theta column number from 2 to %1") .arg(ColumnMinSize))){
            ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    //int Rcol = QInputDialog::getInt(this, "Input",QString("Set R column number from 2 to %1") .arg(ColumnMinSize), 0, 2, ColumnMinSize, 1);
    //int Tcol = QInputDialog::getInt(this, "Input",QString("Set Theta column number from 2 to %1") .arg(ColumnMinSize), 0, 2, ColumnMinSize, 1);

    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.

    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        XY(AllData[0][q],Rcol,Tcol);
    }

    ColumnMinSize = ColumnMinSize +2; //because two new columns have been created.

    // 4- We plot all.
    plotall();

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="37";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(Rcol);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(Tcol);

    ui->statusbar->showMessage(QString("X and Y have been calculated from R and Theta and added as new columns.") ,10000);
}

void MainWindow::on_actionX_Y_R_Theta_triggered(){
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    if(ColumnMinSize < 3){
        QMessageBox::warning(this, "Warning", QString("Datafiles don't have enough columns. A minimum of 3 columns is required.") );
        return;
    }

    int Xcol, Ycol;
    if(!getIntNum(Xcol, 1, ColumnMinSize, QString("Set X column number from 2 to %1") .arg(ColumnMinSize))){
            ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    if(!getIntNum(Ycol, 1, ColumnMinSize, QString("Set Y column number from 2 to %1") .arg(ColumnMinSize))){
            ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }

    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.

    //int Xcol = QInputDialog::getInt(this, "Input",QString("Set X column number from 2 to %1") .arg(ColumnMinSize), 0, 2, ColumnMinSize, 1);
    //int Ycol = QInputDialog::getInt(this, "Input",QString("Set Y column number from 2 to %1") .arg(ColumnMinSize), 0, 2, ColumnMinSize, 1);

    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        RTheta(AllData[0][q],Xcol,Ycol);
    }

    ColumnMinSize = ColumnMinSize +2; //because two new columns have been created.

    // 4- We plot all.
    plotall();

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="36";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(Xcol);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(Ycol);

    ui->statusbar->showMessage(QString("R and Theta have been calculated from X and Y and added as new columns.") ,10000);
}

void MainWindow::on_actionExponenciate_2_triggered(){// Pow to Y axis.
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    // 1.2 - Get input.
    double result;
    if(!getDoubleNum(result, "Set power value: \n(e.g. 1 will lead to no changes, 0.5 will perform square root and 2 will perform square)")){
        return;
    }

    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.


    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        Exponentiate(AllData[0][q],columnPlot,result);
    }

    // 4- We plot all.
    plotall();

    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);
    RememberInv[RememberInv.size()-1][0]=result;


    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 30;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="30";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(result);

    if(columnPlot==0){
        ui->statusbar->showMessage(QString("All columns have been raised to the power of %2.") .arg(columnPlot) .arg(result) ,10000);
    }else{
        ui->statusbar->showMessage(QString("Column %1 has been raised to the power of %2.") .arg(columnPlot) .arg(result) ,10000);
    }
}

void MainWindow::on_actionLogarithm_3_triggered(){ // Log to X axis.
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    double result;
    if(!getDoubleNum(result, "Introduce base of logarithm: \n(e.g. for base 10 introduce 10, for natural log introduce 2.71828182845904523536)")){
        return;
    }

    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.

    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        logarithmX(AllData[0][q],result);
    }

    // 4- We plot all.
    plotall();

    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);
    RememberInv[RememberInv.size()-1][0]=result;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]="29";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(result);

    ui->statusbar->showMessage(QString("Logarithm of base %1 has been applied to Column %2.") .arg(result) .arg(columnPlot),10000);
}

void MainWindow::on_actionLogarithm_4_triggered(){// Log to a Y column.
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    double result;
    if(!getDoubleNum(result, "Introduce base of logarithm: \n(e.g. for base 10 introduce 10, for natural log introduce 2.71828182845904523536)")){
        return;
    }

    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.


    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        logarithm(AllData[0][q],columnPlot,result);
    }

    // 4- We plot all.
    plotall();

    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);
    RememberInv[RememberInv.size()-1][0]=result;

    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 28;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="28";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(result);

    if(columnPlot==0){
        ui->statusbar->showMessage(QString("All columns have been raised to the power of %1.") .arg(result) ,10000);
    }else{
        ui->statusbar->showMessage(QString("Column %1 has been raised to the power of %2.") .arg(columnPlot) .arg(result) ,10000);
    }
}

void MainWindow::on_actionExponenciate_triggered(){// Pow to X axis.
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    double result;
    if(!getDoubleNum(result, "Set power value: \n(e.g. 1 will lead to no changes, 0.5 will perform square root and 2 will perform square)")){
        return;
    }

    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.

    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        ExponentiateX(AllData[0][q],result);
    }

    // 4- We plot all.
    plotall();

    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]="27";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(result);
    RememberInv[RememberInv.size()-1][0]=result;

    ui->statusbar->showMessage(QString("Column 1 (x Axis) has been raised to the power of %1.") .arg(result) ,10000);
}

void MainWindow::on_actionMultiply_constant_3_triggered(){       // It multiplies a constant to X axis.
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    double result;
    if(!getDoubleNum(result, "Set constant value to be multiplied:")){
        return;
    }

    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.

    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        MultiplyConstantX(AllData[0][q],result);
    }

    // 4- We plot all.
    plotall();

    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);
    RememberInv[RememberInv.size()-1][0]=result;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]="44";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(result);

    ui->statusbar->showMessage(QString("Constant %1 has been multiplied to column 1 (x Axis).") .arg(result) ,10000);
}

void MainWindow::on_actionAdd_constant_4_triggered(){       // It adds a constant to X axis.
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    double result;
    if(!getDoubleNum(result, "Set constant value to be added:")){
        return;
    }

    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.

    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        AddConstantX(AllData[0][q],result);
    }

    // 4- We plot all.
    plotall();

    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);
    RememberInv[RememberInv.size()-1][0]=result;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]="26";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(result);

    ui->statusbar->showMessage(QString("Constant %1 has been added to column 1 (x Axis).") .arg(result) ,10000);
}

void MainWindow::on_actionIntegrate_triggered(){
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.

    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        integrate(AllData[0][q], columnPlot);
    }

    // 4- We plot all.
    plotall();

    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 25;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]="25";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);

    ui->statusbar->showMessage(QString("Plots have been integrated.") ,10000);
}

void MainWindow::on_action_Differentiate_triggered(){
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.


    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        differentiate(AllData[0][q], columnPlot);
    }

    // 4- We plot all.
    plotall();

    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 24;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(2);
    AllActions[AllActions.size()-1][0]="24";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);

    ui->statusbar->showMessage(QString("Plots have been derived.") ,10000);
}

void MainWindow::on_actionLaplacian_smoothing_triggered(){
    // 1- We check if files have been opened.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    int times;
    if(!getIntNum(times, 1, 2147483647, QString("How many times do you want to apply the Laplacian algorithm?\n(Suggested value is 1)\n(Higher values will yield curves more smoothed)"))){
            ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    //int times = QInputDialog::getInt(this, "Input",QString("How many times do you want to apply the Laplacian algorithm?\n(Suggested value is 1)\n(Higher values will yield curves more smoothed)") , 1, 1, 2147483647);

    // 2- We make the backup.
    Backup();// It is very important to include the backup BEFORE any data manipulation.

    // 3- We apply the function on the AllData 4Dvector.
    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        LaplacianSmooth(AllData[0][q], columnPlot, times);
    }

    // 4- We plot all.
    plotall();

    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 23;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="23";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(times);

    ui->statusbar->showMessage(QString("Plots have been smoothed.") ,10000);
}

void MainWindow::on_actionClean_interferences_by_FFT_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    //suggested value corresponds to frequencies around 10% of the datarange.
    double suggestion = 10/fabs(AllData[0][0][0][0]-AllData[0][0][0][AllData[0][0][0].size()-1]);
    double lowfreq, highfreq;
    bool error1=1;
    while(error1){

        if(!getDoubleNum(lowfreq, QString("Introduce lowest frequency to filter (an example value is %1).\nHint: To estimate the frequency of the interference to delete subtract the maximum of two consecutive peaks and take its inverse 1/(x_max(i)-x_max(i+1)).") .arg(0.9*suggestion))){
           ui->statusbar->showMessage("No action was performed.",10000);
           return;
        }

        if(!getDoubleNum(highfreq, QString("Introduce maximum frequency to filter (an example value is %1)") .arg(1.1*suggestion))){
            ui->statusbar->showMessage("No action was performed.",10000);
            return;
        }

        if(lowfreq<highfreq){
            error1=0;
        }else{
            QMessageBox::warning(this, "Warning!", "Introduced lowest frequency must be lower than introduced maximum frequency. Please, try again.");
        }
    }

    Backup();

    for(int p=0; p<AllData[0].size();p++){
        Filter(AllData[0][p], columnPlot, lowfreq, highfreq);
    }

    plotall();
    ui->statusbar->showMessage(QString("Oscillations have been filtered from frequencies %1 to %2. The algorithm used is bare Fourier Transform. Faster FFT method will be implemented in future versions.").arg(lowfreq).arg(highfreq),10000);
    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(2);
    RememberInv[RememberInv.size()-1][0]=lowfreq;
    RememberInv[RememberInv.size()-1][1]=highfreq;


    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 20;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(4);
    AllActions[AllActions.size()-1][0]="20";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(lowfreq);
    AllActions[AllActions.size()-1][3]=QString("%1") .arg(highfreq);
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
    plotall();
    ui->statusbar->showMessage(QString("The modulus has been calculated by means of Kramers–Kronig analysis."),10000);

    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 21;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="21";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(previouscolumnPlot);
}

void MainWindow::on_actionAdd_constant_3_triggered(){       // It adds a constant to y axis.

    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    double result;
    if(!getDoubleNum(result, "Set constant value to be added:")){
        return;
    }

    Backup();// It is very important to include the backup BEFORE any data manipulation.

    for(int q=0; q<AllData[0].size(); q++){                                //Run over all files
        AddConstant(AllData[0][q],columnPlot,result);
    }

    plotall();
    ui->statusbar->showMessage(QString("Constant %1 has been added to all plots").arg(result),10000);

    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);

    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 2;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="2";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(result);
    RememberInv[RememberInv.size()-1][0]=result;
    if(columnPlot==0){
        ui->statusbar->showMessage(QString("Constant %1 has been added to all columns.") .arg(result),10000);
    }else{
        ui->statusbar->showMessage(QString("Constant %1 has been added to column #%2.") .arg(result) .arg(columnPlot),10000);
    }

    //QMessageBox::information(this,"Info", QString("AllActions has a size of %1 times %2. A constant of %3 is added to column %4") .arg(AllActions.size()) .arg(AllActions[AllActions.size()-1].size()) .arg(AllActions[AllActions.size()-1][1]) .arg(columnPlot));
}

void MainWindow::on_actionMultiply_constant_4_triggered(){  // It multiplies a constant to y axis.
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }

    double value;
    if(!getDoubleNum(value, "Introduce multiplication value")){
        return;
    }

    Backup();

    for (int q=0; q<AllData[0].size();q++){
        MultConsty3(AllData[0][q], value, columnPlot);
    }
    plotall();

    RememberInv.resize(AllActions.size()+1);
    RememberInv[RememberInv.size()-1].resize(1);
    RememberInv[RememberInv.size()-1][0]=value;

    ActionCol.resize(ActionCol.size()+1); // because this action can be performed over all columns simultaneously
    ActionCol[ActionCol.size()-1] = 15;

    AllActions.resize(AllActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllActions[AllActions.size()-1].resize(3);
    AllActions[AllActions.size()-1][0]="15";
    AllActions[AllActions.size()-1][1]=QString("%1") .arg(columnPlot);
    AllActions[AllActions.size()-1][2]=QString("%1") .arg(value);


    ui->statusbar->showMessage(QString("All plots have been multiplied by %1") .arg(value),10000);
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
    plotall();
    ui->statusbar->showMessage("All data is plotted. Remember that the number of plotted data can be set in 'Graph->Plot->Set max. number of plots'.",10000);
}

void MainWindow::on_actionOnly_first_datafile_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    flag1=0;
    ui->plot->clearGraphs();
    ui->plot->addGraph();
    plotall();
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

    if(!getIntNum(plotNumLim, 0, filenames.size(), QString("Introduce limit of files to be plotted (introduce 0 to plot all files). \nCurrent limit is %1. Current number of opened files is %2.") .arg(plotNumLim) .arg(filenames.size()))){
            ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    //plotNumLim = QInputDialog::getInt(this, "Input",QString("Introduce limit of files to be plotted (introduce 0 to plot all files). \nCurrent limit is %1. Current number of opened files is %2.") .arg(plotNumLim) .arg(filenames.size()),0,0,filenames.size()); //Only takes positive numbers.
    VerticalShift=0; //reset some parameters in case they were manipulated before opening this new files.
    ResetBackup();
    getData(filenames, AllData);
    plotall();
}

void MainWindow::on_actionRescale_triggered(){
    if(filenames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open new files to proceed with this action.") );
        return;
    }
    rescale_rob();
    ui->plot->replot();
    ui->plot->update();
    ui->statusbar->showMessage("Graph rescaled. To zoom in use mouse scroll. Individual axis can be zoomed when mouse is on top.",10000);
}

void MainWindow::on_actionUndo_triggered(){
    if(filenames.isEmpty() && importnames.isEmpty()){
        QMessageBox::warning(this, "Warning", QString("Please, first open or import new files to proceed with this action.") );
        return;
    }

    //if(AllActions.size()>0){
    // First we need to know how many actions can be undone.
    /*
    if(AllActions.size()>numActions){ // if the number of new actions is larger than the previous number of actions
        UndoableActions = AllActions.size() - numActions;
    }
    if(UndoableActions>undo){
        UndoableActions=undo;
    }
    numActions = AllActions.size();*/

//    if(AllActions.size()>0){ //This was not the proper undo condition

   if(!AllData[1].empty()){
//          QMessageBox::information(this, "Information", QString("AllActions size is larger than 0, %1\nThe size of AllData is %2") .arg(AllActions.size()) .arg(AllData.size()) );
//          QMessageBox::information(this, "Information", QString("AllData is empty for: i =0 (%1), 1(%2), 2(%3), 3(%4), 4(%5), 5(%6)") .arg(AllData[0].empty()) .arg(AllData[1].empty()) .arg(AllData[2].empty()) .arg(AllData[3].empty()) .arg(AllData[4].empty()) .arg(AllData[5].empty()) );

        GetBackup();
        plotall();
        //UndoableActions--;
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

    if(AllData[0].size()>1 || columnPlot==0){
        int tempColPlot;
        if(columnPlot==0){
            tempColPlot = 1;
        }else{
            tempColPlot = columnPlot-1;
        }

        double maxVal = AllData[0][0][tempColPlot][0];
        double previousVerticalshift = VerticalShift;

        for(int q=0; q<AllData[0].size();q++){
            for(int w=0; w<AllData[0][q][tempColPlot].size(); w++){
                if(maxVal<AllData[0][q][tempColPlot][w]){
                    maxVal=AllData[0][q][tempColPlot][w];
                }
            }
        }

        if(!getDoubleNum(VerticalShift, QString("Set stacking value to vertically shift the plots. Suggested value is %1") .arg(maxVal))){
            return;
        }

       Backup();

       for(int e = 0; e<AllData[0].size();e++){ // run over files
            VerticalStack(AllData[0][e], e, VerticalShift-previousVerticalshift);
        }

        plotall();

        RememberInv.resize(AllActions.size()+1);
        RememberInv[RememberInv.size()-1].resize(2);
        RememberInv[RememberInv.size()-1][0]=VerticalShift;
        RememberInv[RememberInv.size()-1][1]=previousVerticalshift;

        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(3);
        AllActions[AllActions.size()-1][0]="17";
        AllActions[AllActions.size()-1][1]=QString("%1") .arg(VerticalShift);
        AllActions[AllActions.size()-1][2]=QString("%1") .arg(previousVerticalshift);

        ui->statusbar->showMessage(QString("Plots have been shifted vertically with a stacking constant of %1") .arg(VerticalShift),10000);
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
        plotall();
        ui->statusbar->showMessage(QString("Vertical stack has been undone by a value of %1.") .arg(result),10000);
        AllActions.resize(AllActions.size()+1);
        AllActions[AllActions.size()-1].resize(2);
        AllActions[AllActions.size()-1][0]="18";
        AllActions[AllActions.size()-1][1]=QString("%1") .arg(result);
        RememberInv[RememberInv.size()-1][0]=result;
    }
}

void MainWindow::on_actionlogarithmic_triggered(){

    for (int i = 0; i< plotNum;i++){
        if(columnPlot!=0){
            double* min_y = std::min_element(AllData[0][i][columnPlot-1].begin(), AllData[0][i][columnPlot-1].end());
                if(*min_y<=0){
                    QMessageBox::warning(this, "Warning!", QString("Some datafiles contain negative y values. Log scale cannot be represented.") );
                    return;
                }
        }else{
            for(int q=1; q< AllData[0][i].size();q++){
                double* min_y = std::min_element(AllData[0][i][q].begin(), AllData[0][i][q].end());
                    if(*min_y<=0){
                        QMessageBox::warning(this, "Warning!", QString("Some datafiles contain negative y values. Log scale cannot be represented.") );
                        return;
                    }
            }
        }
    }

    ui->plot->yAxis->setScaleType(QCPAxis::stLogarithmic);
    QSharedPointer<QCPAxisTickerLog> logTicker(new QCPAxisTickerLog);
    log = true;
    ui->plot->yAxis->setTicker(logTicker);
    rescale_rob();
    ui->plot->replot();
    ui->plot->update();
    ui->statusbar->showMessage(QString("Vertical axis scale is now logarithmic.") ,10000);
}

void MainWindow::on_actionLinear_triggered(){
    ui->plot->yAxis->setScaleType(QCPAxis::ScaleType::stLinear);
    QSharedPointer<QCPAxisTicker> mLinTicker (new QCPAxisTicker);
    log = false;
    ui->plot->yAxis->setTicker(mLinTicker);
    rescale_rob();
    ui->plot->replot();
    ui->plot->update();
    ui->statusbar->showMessage(QString("Vertical axis scale is now linear.") ,10000);
}

// *********************************************** FORMAT actions ***********************************************

void MainWindow::on_actionSet_start_of_Col_1_triggered(){
    if(importnames.isEmpty()){ // We check if some files are imported.
        QMessageBox::warning(this, "Warning", QString("Please, first import new files to proceed with this action.") );
        return;
    }

    // Here we check that we can read the files.
    QVector<QVector<QVector<QVector<double>>>> TempArr;
    double start, end;
    for (int q =0; q<importnames.size(); q++){ // We check if we can read the files.
        TempArr.resize(1);
        if(getOneData(importnames[q], TempArr)==1){
            QMessageBox::warning(this,"Warning", QString("Warning: Empty lines or lines with invalid data have been detected in some files. Please, import files with readable format.") );
            return;
        }
        if(q==0){
           start = TempArr[0][0][0][0];
           end = TempArr[0][0][0][TempArr[0][0][0].size()-1];
        }
        TempArr.resize(0);
    }

   double startValue; // To be introduced by the user.
   //bool ok; //If the user does not hit cancell.

   QString message = QString("First imported file starts at value %1 and ends at value %2.\nIntroduce the desired starting value:") .arg(start) .arg(end);
   if(!getDoubleNum(startValue, message)){
       return;
   }
   //QMessageBox::warning(this,"Warning", QString("You have selected %1") .arg(startValue) );

        if(!(((start<startValue)&&(startValue<end))||((start>startValue)&&(startValue>end)))){
            QMessageBox::warning(this,"Warning", QString("Starting value must be between %1 and %2. No action has been perfomed.") .arg(start) .arg(end) );
            return; //We exit the action if no desired row was introduced (cancell) in QInputDialog::getInt.
        }

    AllFormatActions.resize(AllFormatActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllFormatActions[AllFormatActions.size()-1].resize(2);
    AllFormatActions[AllFormatActions.size()-1][0]=QString("11");
    AllFormatActions[AllFormatActions.size()-1][1]=QString("%1").arg(startValue);

    SaveAFA(false); //We only set true for Export, when we want to overwritte the data.

    ui->statusbar->showMessage(QString("%1 datafile(s) have been modified. Select Export (Ctrl+E) to save changes. A preview can be found in folder /TempPreview.") .arg(importnames.size()),10000);
}

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

        if(!getIntNum(ignoreLines, 0, 2147483647, message)){
                ui->statusbar->showMessage("No action was performed.",10000);
            return;
        }
        //ignoreLines = QInputDialog::getInt(this, "Input", message,0,0,2147483647);

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

        if(!getIntNum(ignoreLines, 0,2147483647, message)){
                ui->statusbar->showMessage("No action was performed.",10000);
            return;
        }
        //ignoreLines = QInputDialog::getInt(this, "Input", message,0,0,2147483647);

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
    if(!getIntNum(deletedLines, 0,2147483647, message)){
            ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    //deletedLines = QInputDialog::getInt(this, "Input", message,0,0,2147483647);

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
    if(!getIntNum(deletedLines, 0, 2147483647, message)){
            ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    //deletedLines = QInputDialog::getInt(this, "Input", message,0,0,2147483647);

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
    deletedLines = 0;
    int captionLines =0;

    QString message = QString("Introduce the number of caption rows to be respected.\n(Introduce 0 to delete all non-numerical data rows.)" );
    if(!getIntNum(captionLines, 0, 2147483647, message)){
            ui->statusbar->showMessage("No action was performed.",10000);
        return;
    }
    //captionLines = QInputDialog::getInt(this, "Input", message,0,0,2147483647);

    AllFormatActions.resize(AllFormatActions.size()+1); //I should make a struct where this vector is an object to simplify the whole code.
    AllFormatActions[AllFormatActions.size()-1].resize(2);
    AllFormatActions[AllFormatActions.size()-1][0]=QString("5");
    AllFormatActions[AllFormatActions.size()-1][1]=QString("%1").arg(captionLines);

    SaveAFA(false); //We only set true for Export, when we want to overwritte the data.

 ui->statusbar->showMessage(QString("%1 lines have been deleted in %2 files.  Select Export (Ctrl+E) to save changes. A preview can be found in folder /TempPreview.") .arg(deletedLines) .arg(importnames.size()),10000);
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
           if(!getIntNum(desiredRows, 0, 2147483647, message)){
                   ui->statusbar->showMessage("No action was performed.",10000);
               return;
           }
           //desiredRows = QInputDialog::getInt(this, "Input", message,0,2,2147483647,1,&ok);
       }else{
           QString message = QString("Imported files have different number of rows, from %1 to %2. Introduce the amount of desired rows: \n(Warning! Introducing smaller number of rows will result in loss of information)") .arg(minRows+1) .arg(maxRows+1);

           if(!getIntNum(desiredRows, 0, 2147483647, message)){
                   ui->statusbar->showMessage("No action was performed.",10000);
               return;
           }
           //desiredRows = QInputDialog::getInt(this, "Input", message,0,2,2147483647,1,&ok);
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
        if(!getDoubleNum(desiredStep, QString("The current step of the X variable (1st column) is: %1. \nIntroduce new step:\n(Warning! This action will modify the number of rows of the imported files. If a lower step number is introduced the information of the datafile(s) will be permanently decreased)") .arg(stepnum))){
            return;
        }
    }else if((constant && !equalFiles) || (constant && !start) || (constant && !rowNums) ){
        if(!getDoubleNum(desiredStep, QString("Some warning(s) have been detected: %1%2%3%4 \nThe detected step of the X variable (1st column) is: %5. \nIntroduce new step:\n(Warning! This action will modify the number of rows of the imported files. If a lower step number is introduced the information of the datafile(s) will be permanently decreased).") .arg(message1) .arg(message2) .arg(message3) .arg(message4) .arg(stepnum))){
            return;
        }
    }else{
        int reply = QMessageBox::question(this, "Attention!", "It has been detected that some datafile(s) do not exhibit a constant step of the X variable (1st column). Performing this action is not advised. Are you sure you want to proceed with this action?",
                                      QMessageBox::Yes|QMessageBox::No);
        if (reply == QMessageBox::No) {
            return;
        }
        if(!getDoubleNum(desiredStep, QString("Some warning(s) have been detected: %1%2%3 \nThe current step of the X variable (1st column) is around: %4. \nSince the step value of X is not constant, performing this action is not advised.\nIntroduce new step:\n(Warning! This action will modify the number of rows of the imported files. If a lower step number is introduced the information of the datafile(s) will be permanently decreased).") .arg(message1) .arg(message2) .arg(message3) .arg(stepnum) )){
            return;
        }
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


    if( (New==Replace) || (AppendStart == AppendEnd) || (AppendEnd==Cancel) ){ // only used to dismissed warning about these buttons being unused.
        //do nothing.
    }

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
    msgBox.setText("<center>DataPro v. 1.0.0</center>\n<center>Robert Oliva Vidal</center>\n<center>2020</center>\n<center>This is a beta version, use at your own risk.\n</center><center>You can send comments/suggestions to the whatsapp number +34634119994</center>");
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

//To get a double value form the user use:
//double result;
//if(!getDoubleNum(result, "Set constant value to be added:")){
//        ui->statusbar->showMessage("No action was performed.",10000);
//    return;
//}

//To get an integer value form the user use:
//int result;
//if(!getIntNum(result, "Set constant value to be added:")){
//        ui->statusbar->showMessage("No action was performed.",10000);
//    return;
//}
