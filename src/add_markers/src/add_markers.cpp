#include <ros/ros.h>
#include <visualization_msgs/Marker.h>
#include <geometry_msgs/PoseWithCovarianceStamped.h>


class AMCLPoseSubscriber {
  public:
    AMCLPoseSubscriber() {
      // Subscribe to /amcl_pose topic 
      sub1_ = n_.subscribe("/amcl_pose", 10, &AMCLPoseSubscriber::AMCLPoseCallback, this);
      goalReached = false;
      goalZoneLinTol = 0.2;
    }
    
    bool isGoalReached () {
       return goalReached;
    }

    void setGoalPos (float x, float y) {
      xGoal = x;
      yGoal = y;
      goalReached = false;      
    }

    void AMCLPoseCallback (const geometry_msgs::PoseWithCovarianceStamped::ConstPtr& msg) {
      float xCurr = msg->pose.pose.position.x;
      float yCurr = msg->pose.pose.position.y;

      // if current position is within tolerance of goal position and havent' reached goal yet
      if ( (xCurr >= (xGoal - goalZoneLinTol) ) && (xCurr <= (xGoal + goalZoneLinTol) )
           && (yCurr >= (yGoal - goalZoneLinTol) ) && (yCurr <= (yGoal + goalZoneLinTol) )  
             && goalReached == false ) {
        goalReached = true;
        ROS_INFO("Goal reached");
      } 
    }

  private:
    ros::NodeHandle n_; 
    ros::Subscriber sub1_;
    bool goalReached;
    float xGoal;
    float yGoal;
    float goalZoneLinTol; 
};

void initializeMarkerParams (visualization_msgs::Marker &marker) {

  // Set our initial shape type to be a cube
  uint32_t shape = visualization_msgs::Marker::CUBE;

  // Set the frame ID and timestamp.  See the TF tutorials for information on these.
  marker.header.frame_id = "map";
  marker.header.stamp = ros::Time::now();

  // Set the namespace and id for this marker.  This serves to create a unique ID
  // Any marker sent with the same namespace and id will overwrite the old one
  marker.ns = "basic_shapes";
  marker.id = 0;

  // Set the marker type.  Initially this is CUBE, and cycles between that and SPHERE, ARROW, and CYLINDER
  marker.type = shape;

  // Set the scale of the marker -- 1x1x1 here means 1m on a side
  marker.scale.x = 0.2;
  marker.scale.y = 0.2;
  marker.scale.z = 0.2;

  // Set the color -- be sure to set alpha to something non-zero!
  marker.color.r = 0.0f;
  marker.color.g = 1.0f;
  marker.color.b = 0.0f;
  marker.color.a = 1.0;

  //Set all pose params except position.x and y
  marker.pose.position.z = 0;
  marker.pose.orientation.x = 0.0;
  marker.pose.orientation.y = 0.0;
  marker.pose.orientation.z = 0.0;
  marker.pose.orientation.w = 1.0;

  // Do not autodelete marker
  marker.lifetime = ros::Duration();
  
}

// Set desired marker position
void setMarkerPosition (float x, float y, AMCLPoseSubscriber &AMCLPose_sub, visualization_msgs::Marker &marker) {

  // Set the position of the marker to compare against current position
  AMCLPose_sub.setGoalPos(x, y);

  // Set the X and Y position of the marker
  marker.pose.position.x = x;
  marker.pose.position.y = y;
}

int main( int argc, char** argv )
{
  ros::init(argc, argv, "add_markers");
  ros::NodeHandle n;
  ros::Rate r(1);
  ros::Publisher marker_pub = n.advertise<visualization_msgs::Marker>("visualization_marker", 1);

  // initialize goal positions
  float goalPose[2][2] = {{-1, 1.75}, {-0.75, -3.5}};

  // instantiate subscriber to listen to /amcl_pose topic and compare to current position
  AMCLPoseSubscriber AMCLPose_sub;

  // instantiate marker object
  visualization_msgs::Marker marker;

  // initialize marker parameters
  initializeMarkerParams(marker);

  // FIRST GOAL POSITION (PICKUP)

  setMarkerPosition (goalPose[0][0], goalPose[0][1], AMCLPose_sub, marker);

  // Set the marker action to ADD
  marker.action = visualization_msgs::Marker::ADD;

  while (!AMCLPose_sub.isGoalReached()) {
    marker_pub.publish(marker);

    // Handle ROS communication events (non-blocking)
    ros::spinOnce();
    r.sleep();
  }

  //delete marker (simulate pick up)
  marker.action = visualization_msgs::Marker::DELETE;
  marker_pub.publish(marker);

  // SECOND GOAL POSITION (DROPOFF)

  setMarkerPosition (goalPose[1][0], goalPose[1][1], AMCLPose_sub, marker);

  while (!AMCLPose_sub.isGoalReached()) {
    // Handle ROS communication events
    ros::spinOnce();
    r.sleep();
  }

  // Set the marker action to ADD
  marker.action = visualization_msgs::Marker::ADD;

  // Display marker (drop off)
  while (ros::ok()) {
    marker_pub.publish(marker);
  }
}
