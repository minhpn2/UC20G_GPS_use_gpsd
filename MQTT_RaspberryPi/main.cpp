//  Example C++ gpsd program using GPSD

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <QProcess>
#include <QFileInfo>

extern "C"
{
    #include <gps.h>
}
#include "uc2og_module_function.h"

#define DEBUG

bool UC20_Running_flag = false;
int Return_GPS_data_status = 0;

using namespace UC20G_module;

int main ()
{
    gps_data_t gps_data;

    UC20G_module::UC2OG_module_function UC20G;

    // Start GPS Module
    UC20_Running_flag = UC20G.Initialize_UC20G();

    // Open Port Data GPS
    UC20G.OpenPort_Data_GPS(&gps_data, UC20_Running_flag, &Return_GPS_data_status);

    //loop
    for(;;)
    {
//        UC20G.OpenPort_Data_GPS(&gps_data, UC20_Running_flag, &Return_GPS_data_status);

        bool as = UC20G.GetData_GPS(&gps_data, UC20_Running_flag, &Return_GPS_data_status);

        if (Return_GPS_data_status == UC20G.HAVE_DATA_GPS)
        {
#ifdef DEBUG
            qDebug() << "SUCCESS!!!";
#endif
        }

        else if (Return_GPS_data_status == UC20G.NO_DATA_GPS)
        {
            qDebug() << "No signal GPS!!!";
        }
        else if ((Return_GPS_data_status == UC20G.GPS_MODULE_RESETED)
                 && (UC20_Running_flag == true))
        {
#ifdef DEBUG
            qDebug() << "Initialize GPS Module again!!!";
#endif
            UC20G.Initialize_UC20G_After_reseted(&gps_data, UC20_Running_flag, &Return_GPS_data_status);
        }
        else if(Return_GPS_data_status == UC20G.HAVE_PROBLEM)
        {
            qDebug() << "HAVE PROBLEM";
        }
        sleep(1); // Delay 2 seconds
    }

    // return empty data
    return 0;
}

