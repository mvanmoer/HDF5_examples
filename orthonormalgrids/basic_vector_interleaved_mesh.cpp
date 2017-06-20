// Makng a basic mesh with HDF5.
// This is analogous to a vtkImageData dataset.
// There is one trivial vector field "vectors." 
// The vector components are packed into three separate
// std::vector<float>'s which are then written as if
// they were individual scalars. An XDMF join operation
// will combine them.
// VisIt will not load the .h5, but will need to be
// given basic_vector_mesh.xdmf, which is hand-written.

#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include "hdf5.h"

int main(int argc, char** argv) {
    const std::string filename("basic_vector_interleaved_mesh.h5");
    std::cout << "Output filename is: " << filename << std::endl;

    hid_t file; 
    hid_t dataset; 
    hid_t dataspace;

    herr_t status;

    // HDF5 convention is fastest is last, this is {zdim, ydim, xdim, numcomps}.
    hsize_t dims[4] = { 128, 192, 256, 3 };
    
    // arbitrary vector data for writing.
    std::vector<float> vectors;
    for (int k = 0; k < dims[0]; k++) {
        for (int j = 0; j < dims[1]; j++) {
            for (int i = 0; i < dims[2]; i++) {
                vectors.push_back(1.0f/i);
                vectors.push_back(1.0f/j);
                vectors.push_back(1.0f/k);
            }
        }
    }

    // Create the file handle. 
    file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    dataspace = H5Screate_simple(4, dims, NULL);
     
    dataset = H5Dcreate2(file, "/vectors", H5T_IEEE_F32LE, dataspace,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &vectors.front());
    status = H5Sclose(dataspace);
    status = H5Dclose(dataset);

    status = H5Fclose(file);
    return 0;
}
