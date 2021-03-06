#ifndef ecCytonCommands_H_
#define ecCytonCommands_H_
//------------------------------------------------------------------------------
// Copyright (c) 2004-2013 Energid Technologies. All rights reserved.
//
/// @file ecCytonCommands.h
/// @class EcNetworkCommunicationTester
/// @brief Holds the quick-start code described in the Users Guide.
//
//------------------------------------------------------------------------------
#include <foundCore/ecTypes.h>
#include <foundCommon/ecCoordSysXForm.h>
#include <UAV_mani/mavlink.h>
#include <pthread.h>
#include "autopilot_interface.h"
#include <control/ecManipEndEffectorPlace.h>

void* read_armstatus_thread(void*args);
/// This class uses the remote commands API to communicate with the 
//  ActinViewer/CytonViewer or ActinRT with the remoteCommandServerPlugin loaded.
class EcCytonCommands
{
public:

   /// constructor
   EcCytonCommands
      (
      );
   EcCytonCommands
      (Autopilot_Interface *autopilot_interface_
      );
   /// destructor
   virtual ~EcCytonCommands
      (
      );

   /// copy constructor
   EcCytonCommands
      (
      const EcCytonCommands& orig
      );

   /// overloading = operator
    EcCytonCommands& operator=
      (
       EcCytonCommands& orig
      );

   /// overloading == operator
   EcBoolean operator==
      (
      const EcCytonCommands& orig
      )const;

   /// initialize the network
   /// @param[in] ipAddress          (EcString&) address of the network to be connected to.
   /// @return                       (EcBoolean) which returns the status of command
   virtual EcBoolean openNetwork
      (
      const EcString& ipAddress
      )const;

   /// shut down the network
   /// @return    flag (EcBoolean) which returns the status of command
   virtual EcBoolean closeNetwork
      (
      )const;

   /// test joint values communication over the network
   /// @param[in] jointPosition (EcRealVector&) desired joint state
   /// @param[in] angletolerance (EcReal&) desired joint state precision
   /// @return    flag  (EcBoolean) which returns the status of command
   virtual EcBoolean MoveJointsExample
      (
      const EcRealVector jointPosition,
      const EcReal angletolerance
      );


   /// Move the gripper on the server side
   /// @param[in] gripperPos  (EcReal) desired gripper position
   /// @return    flag              (EcBoolean) which returns the status of command
   virtual EcBoolean moveGripperExample
      (
      const EcReal gripperPos
      );
   /// move the robot using point EE set (only constrains position (x,y,z))
   /// @param[in] pose (EcCoordinateSystemTransformation&) desired pose
   /// @return         (EcBoolean) flag which returns the status of command
   virtual EcBoolean pointMovementExample
      (const EcCoordinateSystemTransformation &pose)const;

   /// move the robot using frame EE set (constrains x,y,z, and roll, pitch, yaw)
   /// @param[in] pose (EcCoordinateSystemTransformation&) desired pose
   /// @return         (EcBoolean) flag which returns the status of command
   virtual EcBoolean frameMovementExample
      (float x,float y,float z,float roll,float pitch,float yaw);

   /// move the end effector with the desired end effector velocity for one second
   /// @param[in] endVelo (EcRealVector&) desired velocity direction and magnitude [x,y,z] 
   virtual EcBoolean endEffectorVelocityTest
      (
           const EcRealVector& endVelo
           )const;

   ///sample path planning to get to the desired pose
   // @param[in] pose (EcCoordinateSystemTransformation&) desired Pose
   /// @return         (EcBoolean) flag which returns the status of command
   virtual EcBoolean pathPlanningExample
      (float x, float y, float z);

   /// enable hardware test
   /// @param[in] flag     (EcBoolean) Whether to enable/disable hardware
   /// @return    flag     (EcBoolean)which returns the status of command
   virtual EcBoolean hardwareEnableTest
      (
      const EcBoolean flag
      )const;

   /// move the robot to home position(zero joint angles for all the joints)
   virtual EcBoolean resetToHome
       (
       );

  /*set the target end-eff frame to targFrame
   * while lock autopilot_interface.target_endeff_frame*/
   virtual EcBoolean setTargFrame();


   /*get the all status of the Epsilon 300*/
   virtual EcBoolean getFrameStatus(EcCoordinateSystemTransformation coordStatus);
   virtual EcBoolean getJoinStatus(EcRealVector currJoin);

   /*update  autopilot_interface.endeff_frame_status and
    * autopilot_interface.mani_joints the from frameStatus and joinStatus
    *  while lock autopilot_interface */
   virtual EcBoolean updateFrameStatus();
   virtual EcBoolean updateJoinStatus();

   char reading_status;
   /*targFrame commands from pixhawk*/
   mavlink_target_endeff_frame_t targFrame;
   pthread_t read_armstatus_tid;
   pthread_mutex_t actualEEP_lock;
   pthread_mutex_t actual_joint_lock;

   void read_armstatus_thread_main();
   void start();
   void stop();
   void handle_quit( int sig );
protected:
   /* Status of Epsilon300 arm*/
   EcManipulatorEndEffectorPlacement actualEEPlacement;
   EcRealVector currentJoints;

   Autopilot_Interface *autopilot_interface;

   /* frameStatus of Epsilon 300 need to send to pixhawk*/
   mavlink_endeff_frame_status_t frameStatus;
   mavlink_manipulator_joint_status_t joinStatus;
   bool time_to_exit;
};

#endif //ecCytonCommands_H_
