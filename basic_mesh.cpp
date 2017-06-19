// Making a basic mesh with HDF5.
// This is analogous to a vtkImageData dataset.
// There is one trivial scalar field "scalars" and
// one trivial vector field "vectors."
// The saved file, basic_mesh.h5 can be read correctly
// by VisIt's Pixie reader.

#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include "hdf5.h"

const std::string filename("basic_mesh.h5");

int main(int argc, char** argv) {
    std::cout << "Output filename is: " << filename << std::endl;

    hid_t file; 
    hid_t dataset; 
    hid_t dataspace;

    herr_t status;

    // HDF5 convention is fastest is last, this is {zdim, ydim, xdim}.
    hsize_t dims[3] = { 128, 192, 256 };
    std::vector<float> scalars;

    float total = 256 * 192;
    for (int k = 0; k < dims[0]; k++) {
        for (int j = 0; j < dims[1]; j++) {
            for (int i = 0; i < dims[2]; i++) {
                float x = float(i)/(atan(1.0) * dims[2]);
                float y = 2.0f * float(j)/dims[1];
                scalars.push_back(sinf(x) * cosf(y));
            }
        }
    }
    
    // https://support.hdfgroup.org/HDF5/doc/RM/RM_HF5.html#File-Create
    // Just this function will create an 800 byte HDF5 file which
    // contains an empty root group.
    // args: 
    // file name
    // truncate (overwrite) file or H5F_ACC_EXCL to fail if already exists
    // file creation property list, using default
    // file access property list, using default
    file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);


    // https://support.hdf5group.org/HDF5/doc/RM/RM_H5S.html#Dataspace-CreateSimple
    // args:
    // rank
    // current dimensions
    // maximum dimensions, if NULL, use current.
    dataspace = H5Screate_simple(3, dims, NULL);

    // https://support.hdfgroup.org/HDF5/doc/RM/RM_H5D.html#Dataset-Create
    // args:
    // file or group id for placing dataset
    // human readable name of {absolute|relative} path to dataset
    // data type (IEEE single precision little-endian float)
    // data space
    // link creation property list
    // dataset creation property list
    // dataset access proprerty list
    dataset = H5Dcreate2(file, "/scalars", H5T_IEEE_F32LE, dataspace,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &scalars.front());

    status = H5Dclose(dataset);
    status = H5Sclose(dataspace);
    status = H5Fclose(file);
    return 0;
}


