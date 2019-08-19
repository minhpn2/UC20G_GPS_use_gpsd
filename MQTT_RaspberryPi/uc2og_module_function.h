#ifndef UC2OG_MODULE_FUNCTION_H
#define UC2OG_MODULE_FUNCTION_H

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <QProcess>
#include <wiringPi.h>
#include <QDebug>
#include <QDir>
#include "math.h"
#include "gps.h"

#define RaspberryPi_GPS_Start 31   // Pin 31 of Raspberry Pi
#define RaspberryPi_GPS_Reset 30   // Pin 30 of Raspbeery Pi
#define DelayTimeStartMS      10   // 10 ms
#define DelayTimeResetMS      10   // 10 ms
#define NUMBER_LOOP_WAIT_CREATED_UC20_COMMAND INT_MAX //2147483647
#define path_UC20_COMMAND "/dev/ttyUSB_UC20_COMMAND"
#define path_UC20_DATA    "/dev/ttyUSB_UC20_DATA"

namespace UC20G_module
{
    // Class chua cac chuc nang auto tim port de connect cua UC20G
    class UC2OG_module_function
    {
    public:
        enum GPS_Running_status
        {
          HAVE_DATA_GPS = 0,
          NO_DATA_GPS = 1,
          NO_SIGNAL_GPS = 2,
          GPS_MODULE_RESETED = 3,
          HAVE_PROBLEM = 4
        };
    public:

        UC2OG_module_function(); // Intialize conductor

        virtual ~UC2OG_module_function(); // Virtual conductor

    public:
        // Chuong trinh khoi dong UC20G module
        bool Initialize_UC20G();

        // This function will open GPS port when it called
        void OpenPort_Data_GPS(gps_data_t* gps_data, bool GPS_Running_flag, int *Return_GPS_data_status);

        // This function will close GPS port when it called
        void ClosePort_Data_GPS(gps_data_t *gps_data);

        // This functon will get data of GPS module and return data via struct gps_data.
        bool GetData_GPS(gps_data_t *gps_data, bool GPS_Running_flag, int *GPS_Running_status);

        // This function used only when GPS Module was reseted
        void Initialize_UC20G_After_reseted(gps_data_t *gps_data, bool UC20_Running_flag, int *Return_GPS_data_status);
        // Chuong trinh start GPS
        void Start_GPS_Module(bool GPS_Running_flag);

        // Chuong trinh Turn off GPS
        void Turnoff_GPS_Module(bool GPS_Runnging_flag);

        // Chuong trinh Reset GPS
        void Reset_GPS_Module(bool GPS_Runnging_flag);

        // Chuong trinh khoi tao cac chuc nang cho UC20G
    };
}
#endif // UC2OG_MODULE_FUNCTION_H
