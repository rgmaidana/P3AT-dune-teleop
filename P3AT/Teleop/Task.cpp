//***************************************************************************
// Copyright 2007-2019 Universidade do Porto - Faculdade de Engenharia      *
// Laboratório de Sistemas e Tecnologia Subaquática (LSTS)                  *
//***************************************************************************
// This file is part of DUNE: Unified Navigation Environment.               *
//                                                                          *
// Commercial Licence Usage                                                 *
// Licencees holding valid commercial DUNE licences may use this file in    *
// accordance with the commercial licence agreement provided with the       *
// Software or, alternatively, in accordance with the terms contained in a  *
// written agreement between you and Faculdade de Engenharia da             *
// Universidade do Porto. For licensing terms, conditions, and further      *
// information contact lsts@fe.up.pt.                                       *
//                                                                          *
// Modified European Union Public Licence - EUPL v.1.1 Usage                *
// Alternatively, this file may be used under the terms of the Modified     *
// EUPL, Version 1.1 only (the "Licence"), appearing in the file LICENCE.md *
// included in the packaging of this file. You may not use this work        *
// except in compliance with the Licence. Unless required by applicable     *
// law or agreed to in writing, software distributed under the Licence is   *
// distributed on an "AS IS" basis, WITHOUT WARRANTIES OR CONDITIONS OF     *
// ANY KIND, either express or implied. See the Licence for the specific    *
// language governing permissions and limitations at                        *
// https://github.com/LSTS/dune/blob/master/LICENCE.md and                  *
// http://ec.europa.eu/idabc/eupl.html.                                     *
//***************************************************************************
// Author: renan                                                            *
//***************************************************************************

// DUNE headers.
#include <DUNE/DUNE.hpp>

// Standard GCC headers
#include <signal.h>
#include <stdio.h>
#include <termios.h>

// Keyboard codes
#define KEYCODE_R 0x43
#define KEYCODE_L 0x44
#define KEYCODE_U 0x41
#define KEYCODE_D 0x42
#define KEYCODE_A 0x61
#define KEYCODE_Z 0x7A
#define KEYCODE_S 0x73
#define KEYCODE_X 0x78
#define KEYCODE_Q 0x71
#define KEYCODE_SPACE 0x20

int kfd = 0;
struct termios cooked, raw;

void handle_sigint(int sig){
  tcsetattr(kfd, TCSANOW, &cooked);
  exit(0);
}

namespace P3AT
{
  //! Insert short task description here.
  //!
  //! Insert explanation on task behaviour here.
  //! @author renan
  namespace Teleop
  {
    using DUNE_NAMESPACES;

    struct Task: public DUNE::Tasks::Task
    {
      //! Constructor.
      //! @param[in] name task name.
      //! @param[in] ctx context.
      Task(const std::string& name, Tasks::Context& ctx):
        DUNE::Tasks::Task(name, ctx)
      {
      }

      //! Update internal state with new parameter values.
      void
      onUpdateParameters(void)
      {
      }

      //! Reserve entity identifiers.
      void
      onEntityReservation(void)
      {
      }

      //! Resolve entity names.
      void
      onEntityResolution(void)
      {
      }

      //! Acquire resources.
      void
      onResourceAcquisition(void)
      {
      }

      //! Initialize resources.
      void
      onResourceInitialization(void)
      {
      }

      //! Release resources.
      void
      onResourceRelease(void)
      {
      }

      //! Main loop.
      void
      onMain(void)
      {
        double linear_ = 0, angular_ = 0;
        double current_linear_ = 0.1, current_angular_ = 0.1;
        double step_linear_ = 0.2, step_angular_ = 0.1;
        
        char c;
        bool dirty=false, quit=false;
        // get the console in raw mode
        tcgetattr(kfd, &cooked);
        memcpy(&raw, &cooked, sizeof(struct termios));
        raw.c_lflag &=~ (ICANON | ECHO);
        // Setting a new line, then end of file
        raw.c_cc[VEOL] = 1;
        raw.c_cc[VEOF] = 2;
        tcsetattr(kfd, TCSANOW, &raw);
        inf("Reading from keyboard");
        inf("---------------------------");
        inf("Use arrow keys to move the robot.");
        inf("Press the space bar to stop the robot.");
        inf("Press q to quit the program.");
        inf("a/z - Increase/decrease linear velocity");
        inf("s/x - Increase/decrease angular velocity");

        signal(SIGINT, handle_sigint);

        // Run while dune task is active
        while (!stopping())
        {
          // get the next event from the keyboard
          if(read(kfd, &c, 1) < 0)
          {
            perror("read():");
            exit(-1);
          }
          linear_ = 0; 
          angular_ = 0;
          
          switch(c)
          {
            case KEYCODE_L:
              //inf("LEFT");
              angular_ = current_angular_;
              linear_ = 0;
              dirty=true;
              break;
            case KEYCODE_R:
              //inf("RIGHT");
              angular_ = -current_angular_;
              linear_ = 0;
              dirty=true;
              break;
            case KEYCODE_U:
              //inf("UP");
              linear_ = current_linear_;
              angular_ = 0;
              dirty=true;
              break;
            case KEYCODE_D:
              //inf("DOWN");
              linear_ = -current_linear_;
              angular_ = 0;
              dirty=true;
              break;
            case KEYCODE_A:
              //inf("INCREASE LINEAR SPEED");
              current_linear_ += step_linear_;
              inf("Linear speed: %02f", current_linear_);
              dirty=true;
              break;
            case KEYCODE_Z:
              //inf("DECREASE LINEAR SPEED");
              current_linear_ -= step_linear_;
              if(current_linear_ < 0)
                current_linear_ = 0;
              inf("Linear speed: %02f", current_linear_);
              dirty=true;
              break;
            case KEYCODE_S:
              //inf("INCREASE ANGULAR SPEED");
              current_angular_ += step_angular_;
              inf("Angular speed: %02f", current_angular_);
              dirty=true;
              break;
            case KEYCODE_X:
              //inf("DECREASE LINEAR SPEED");
              current_angular_ -= step_angular_;
              if(current_angular_ < 0)
              current_angular_ = 0;
              inf("Angular speed: %02f", current_angular_);
              dirty=true;
              break;
            case KEYCODE_SPACE:
              //inf("STOP");
              linear_ = 0;
              angular_ = 0;
              dirty=true;
              break;
            case KEYCODE_Q:
              inf("Quitting...");
              quit=true;
            default: break;
          }

          IMC::DesiredVelocity robotVel;    // Velocity message for P3AT robot
          robotVel.u = linear_;
          robotVel.r = angular_;

          if(quit==true){
            // Stop the robot
            robotVel.u = 0;
            robotVel.r = 0;
            dispatch(robotVel);

            handle_sigint(SIGINT);
          }

          if (dirty==true){
            dispatch(robotVel);
            dirty=false;
          }

          //Delay::wait(0.1);    // Sleep for 100 ms (10 Hz)
        }
      }
    };
  }
}

DUNE_TASK
