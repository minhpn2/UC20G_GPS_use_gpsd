#include "uc2og_module_function.h"

namespace UC20G_module
{
    UC2OG_module_function::UC2OG_module_function()
    {
        // TODO run conductor
    }

    UC2OG_module_function::~UC2OG_module_function()
    {
        // TODO run detructor
    }

    // Chuong trinh khoi dong UC20G Module
    bool UC2OG_module_function::Initialize_UC20G()
    {
        /*
         * Khoi dong UC20G module by set up pin 31 of raspberry Pi
        */
        bool GPS_Running_flag = false;
        Start_GPS_Module(GPS_Running_flag);
        /*
         * Checking the file UC20_COMMAND created?
         * Solution:
         * - Using QFileInfo for checking file UC20_COMMAND
         * - Waitting for file UC20_COMMAND created.
        */
        QFileInfo dir(path_UC20_COMMAND);
        for(int i = 0;i < NUMBER_LOOP_WAIT_CREATED_UC20_COMMAND;i++)
        {
            if(!dir.exists())
            {
#ifdef Debug_uc20g_init
                qDebug() << "File UC20_COMMAND still is not created...\n";
#endif
                // The GPS is not running, set GPS_Running_flag flag false.
                GPS_Running_flag = false;
            }
            // Neu link ton tai
            else if(dir.exists())
            {
#ifdef Debug_uc20g_init
                qDebug() << "GPS module started!!!";
#endif
                try
                {
#ifdef Debug_uc20g_init
                    qDebug() << "Initilize GPS...";
#endif
                    QProcess sh;
                    sh.start("sh",QStringList() << "-c" << "echo \"AT+QGPS=1\" > /dev/ttyUSB_UC20_COMMAND",QIODevice::ReadWrite);
                    sh.waitForStarted();
                    sh.execute("gpsd /dev/ttyUSB_UC20_DATA –F /var/run/gpsd.sock");
                    sh.waitForFinished();
                    sh.close();

                    // GPS Module da duoc khoi dong xong
                    GPS_Running_flag = true;

                    // Exit loop
                    break;
                }
                catch (...)
                {
#ifdef Debug_uc20g_init
                    qDebug() << "Cannot initilize GPS Module!!!";
#endif
                    // GPS Module da duoc khoi dong xong
                    GPS_Running_flag = false;
                    // Exit all program
                    break;
                }
            }
        }
        // Return flag for report the status of GPS Module
        return GPS_Running_flag;

    }

    void UC2OG_module_function::OpenPort_Data_GPS(gps_data_t *gps_data, bool GPS_Running_flag, int *Return_GPS_data_status)
    {
        if(GPS_Running_flag == true)
        {
            if(gps_open("localhost", "2947", gps_data) == -1)
            {
#ifdef Debug_uc20g_init
                qDebug() << "Having problem when open GPS port data!!!";
#endif
                *Return_GPS_data_status = NO_SIGNAL_GPS;
            }
            else
            {
                gps_stream(gps_data, WATCH_ENABLE | WATCH_JSON, nullptr);
            }
        }
    }

    void UC2OG_module_function::ClosePort_Data_GPS(gps_data_t *gps_data)
    {
        gps_stream(gps_data, WATCH_DISABLE, nullptr);
        gps_close(gps_data);
    }

    bool UC2OG_module_function::GetData_GPS(gps_data_t *gps_data, bool GPS_Running_flag, int *Return_GPS_data_status)
    {
        try
        {
            /* Checking the pointer to Gps_data array exists and Status of GPS Module*/
            if ((gps_data != nullptr) && (GPS_Running_flag == true)
                 && (gps_waiting(gps_data, 2000)))
            {
                // Initialize value contains returned data of gps_read() function
                int rc;
                /* read data GPS*/
                if ((rc = gps_read(gps_data)) == -1)
                {
#ifdef Debug_uc20g_init
                    qDebug("Error occured reading gps data. code: %d, reason: %s\n", rc, gps_errstr(rc));
#endif
                    *Return_GPS_data_status = GPS_MODULE_RESETED;
                    return false;
                }
                else
                {
                    /* Display data from the GPS receiver. */
                    if ((gps_data->status == STATUS_FIX) &&
                        (!isnan(gps_data->fix.latitude) |
                         !isnan(gps_data->fix.longitude)))
                    {
#ifdef Debug_uc20g_init
                        qDebug("latitude: %f, longitude: %f, speed: %f, timestamp: %lf\n", gps_data->fix.latitude, gps_data->fix.longitude, gps_data->fix.speed, gps_data->fix.time); //EDIT: Replaced tv.tv_sec with gps_data.fix.time
#endif
                        *Return_GPS_data_status = HAVE_DATA_GPS;
                        return true;
                     }
                     else
                     {
                        *Return_GPS_data_status = NO_DATA_GPS;
                        return false;
                     }
                 }
            }
            else
            {
                *Return_GPS_data_status = GPS_MODULE_RESETED;
                return false;
            }

        }
        catch (...)
        {
            // Error occurs when run this function
            *Return_GPS_data_status = HAVE_PROBLEM;
            return false;
        }
    }

    void UC2OG_module_function::Initialize_UC20G_After_reseted(gps_data_t *gps_data,bool UC20_Running_flag, int *Return_GPS_data_status)
    {
        try
        {
            if((*Return_GPS_data_status == GPS_MODULE_RESETED)
                && (UC20_Running_flag == true))
            {
#ifdef Debug_uc20g_init
                    qDebug() << "Initilize GPS...";
#endif
                    QProcess sh;
                    sh.start("sh",QStringList() << "-c" << "echo \"AT+QGPS=1\" > /dev/ttyUSB_UC20_COMMAND",QIODevice::ReadWrite);
                    sh.waitForStarted();
                    sh.execute("gpsd /dev/ttyUSB_UC20_DATA –F /var/run/gpsd.sock");
                    sh.waitForFinished();
                    sh.close();

                    if(gps_open("localhost", "2947", gps_data) == -1)
                    {
#ifdef Debug_uc20g_init
                        qDebug() << "Having problem when open GPS port data!!!";
#endif
                        *Return_GPS_data_status = HAVE_PROBLEM;
                    }
                    else
                    {
                        gps_stream(gps_data, WATCH_ENABLE | WATCH_JSON, nullptr);
                    }
                    // GPS Module da duoc khoi dong xong
                    UC20_Running_flag = true;
                    *Return_GPS_data_status = NO_DATA_GPS;
            }
            else
            {
                // TO DO: nothing
            }

        }
        catch (...)
        {
            // Program have problem, request stop all function for GPS_Module
            *Return_GPS_data_status = HAVE_PROBLEM;
            UC20_Running_flag = false;
        }
    }

    void UC2OG_module_function::Start_GPS_Module(bool GPS_Running_flag)
    {
        wiringPiSetup(); // Configure GPIO on raspberry pi
        pinMode(RaspberryPi_GPS_Start, OUTPUT); // Dinh nghia chan 31 cua Raspberry: Output

        if (!GPS_Running_flag)
        {
            // GPS chi khoi dong khi co xung len tai chan RaspberryPi_GPS_Start
            digitalWrite(RaspberryPi_GPS_Start, LOW);
            delayMicroseconds(DelayTimeStartMS);
            digitalWrite(RaspberryPi_GPS_Start, HIGH);
        }
        else
        {
            // TO_DO: nothing
        }
    }

    void UC2OG_module_function::Reset_GPS_Module(bool GPS_Runngin_flag)
    {
        wiringPiSetup();
        pinMode(RaspberryPi_GPS_Reset, OUTPUT);
        if(GPS_Runngin_flag)
        {
            // Set off-->on---> off chan Reset cua GPS trong mot thoi gian
            digitalWrite(RaspberryPi_GPS_Reset, LOW);
            delayMicroseconds(DelayTimeResetMS);
            digitalWrite(RaspberryPi_GPS_Reset, HIGH);
            delayMicroseconds(DelayTimeResetMS);
            digitalWrite(RaspberryPi_GPS_Reset, LOW);
        }
        else
        {
            // TO_DO: nothing
        }
    }

    void UC2OG_module_function::Turnoff_GPS_Module(bool GPS_Runngin_flag)
    {
        wiringPiSetup(); // Configure GPIO on raspberry pi
        pinMode(RaspberryPi_GPS_Start, OUTPUT); // Dinh nghia chan 31 cua Raspberry: Output

        // GPS chi khoi dong khi co xung len tai chan RaspberryPi_GPS_Start
        if(GPS_Runngin_flag)
        {
            digitalWrite(RaspberryPi_GPS_Start, HIGH);
            delayMicroseconds(DelayTimeStartMS);
            digitalWrite(RaspberryPi_GPS_Start, LOW);
        }
    }

    // Chuong trinh auto tim port giao tiep command
}

