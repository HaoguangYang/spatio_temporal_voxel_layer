#include <pcl/io/pcd_io.h>
#include <pcl/console/print.h>
#include <pcl/console/parse.h>
#include <boost/algorithm/string/split.hpp> // for split
#include <spatio_temporal_voxel_layer/vdb2pc.hpp>

using namespace pcl;
using namespace pcl::io;
using namespace pcl::console;

void
printHelp (int, char **argv)
{
  print_error ("Syntax is: %s input.vdb output.pcd\n", argv[0]);
}

bool
loadCloud (const std::string &filename, PointCloud<PointXYZ>::Ptr &cloud)
{
  utilities::VDB2PCLPointCloud cvt = utilities::VDB2PCLPointCloud();
  cvt.SetFile(filename);
  return cvt.GetCloud(cloud);
}

/* ---[ */
int
main (int argc, char** argv)
{
  print_info ("Convert a VDB file to PCD format. For more information, use: %s -h\n", argv[0]);

  if (argc < 3)
  {
    printHelp (argc, argv);
    return (-1);
  }

  // Parse the command line arguments for .pcd and .ply files
  std::vector<int> pcd_file_indices = parse_file_extension_argument (argc, argv, ".pcd");
  std::vector<int> vdb_file_indices = parse_file_extension_argument (argc, argv, ".vdb");
  if (pcd_file_indices.size () != 1 || vdb_file_indices.size () != 1)
  {
    print_error ("Need one input VDB file and one output PCD file.\n");
    return (-1);
  }

  // Load the first file
  PointCloud<PointXYZ>::Ptr cloudPTR(new PointCloud<PointXYZ>);
  if (!loadCloud (argv[vdb_file_indices[0]], cloudPTR)) 
    return (-1);

  // Convert to PCD and save
  PCDWriter w;
  w.writeBinaryCompressed (argv[pcd_file_indices[0]], *cloudPTR);
}