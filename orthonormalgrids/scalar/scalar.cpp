// Making an orthonormal mesh with HDF5.
// This is analogous to a vtkImageData dataset.
// There is one trivial scalar field "scalars." 
// The saved file, scalar.h5 can be read directly 
// by VisIt's Pixie reader, or with scalar.xdmf.

#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include "hdf5.h"

int main(int argc, char** argv) {
    using std::cout;
    using std::endl;

    const std::string filename("scalar.h5");
    cout << "Output filename is: " << filename << endl;

    hid_t file; 
    hid_t dataset; 
    hid_t dataspace;

    herr_t status;

    // HDF5 convention is fastest is last, this is {zdim, ydim, xdim}.
    hsize_t dims[3] = { 128, 192, 256 };
    
    // arbitrary scalar data for writing.
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
    
    // args: 
    // file name
    // truncate (overwrite) file or H5F_ACC_EXCL to fail if already exists
    // file creation property list, using default
    // file access property list, using default
    cout << "Calling H5Fcreate..." << endl;
    file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    // args:
    // rank
    // current dimensions
    // maximum dimensions, if NULL, use current.
    cout << "Calling H5Screate..." << endl;
    dataspace = H5Screate_simple(3, dims, NULL);

    // file or group id for placing dataset
    // human readable name of {absolute|relative} path to dataset
    // data type (IEEE single precision little-endian float)
    // data space
    // link creation property list
    // dataset creation property list
    // dataset access proprerty list
    cout << "Calling H5Dcreate2..." << endl;
    dataset = H5Dcreate2(file, "/scalars", H5T_IEEE_F32LE, dataspace,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    // args:
    // dataset
    // memory datatype - this is different than file datatype
    // what part of dataset memory to write (all of it)
    // file data space
    // transfer property list
    // pointer to data
    cout << "Calling H5Dwrite..." << endl;
    status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &scalars.front());

    cout << "Closing dataset, dataspace, and file..." << endl;
    status = H5Dclose(dataset);
    status = H5Sclose(dataspace);
    status = H5Fclose(file);
    return 0;
}


