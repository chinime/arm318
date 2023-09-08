search_mode=OPTIMIZE_MAX_JOINT
srdf_filename=six_dof_spatial_manipulator.srdf
robot_name_in_srdf=six_dof_spatial_manipulator
moveit_config_pkg=six_dof_spatial_manipulator_moveit_config
robot_name=six_dof_spatial_manipulator
planning_group_name=five_dof_planning_group
ikfast_plugin_pkg=six_dof_spatial_manipulator_five_dof_planning_group_ikfast_plugin
base_link_name=base_link
eef_link_name=endeff1
ikfast_output_path=/home/chinime/catkin_ws/six_dof_spatial_manipulator_five_dof_planning_group_ikfast_plugin/src/six_dof_spatial_manipulator_five_dof_planning_group_ikfast_solver.cpp

rosrun moveit_kinematics create_ikfast_moveit_plugin.py\
  --search_mode=$search_mode\
  --srdf_filename=$srdf_filename\
  --robot_name_in_srdf=$robot_name_in_srdf\
  --moveit_config_pkg=$moveit_config_pkg\
  $robot_name\
  $planning_group_name\
  $ikfast_plugin_pkg\
  $base_link_name\
  $eef_link_name\
  $ikfast_output_path
