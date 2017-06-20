// Making a rectilinear mesh with HDF5.
// This is analogous to a vtkRectilinearGrid dataset.
// There is one trivial scalar field "scalars." 

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
    hsize_t xdim = 256;
    hsize_t ydim = 192;
    hsize_t zdim = 128;
    hsize_t dims[3] = { zdim, ydim, xdim };
   
    // Arrays for grid spacing along each axis.
    float ord = 0.0f;
    std::vector<float> xcoords;
    for (int i = 0; i < xdim; i++) {
        xcoords.push_back(ord);
        ord += i * 0.1f;
    }
    ord = 0.0f;
    
    std::vector<float> ycoords;
    for (int i = 0; i < ydim; i++) {
        ycoords.push_back(ord);
        ord += i * 0.1f;
    }
    ord = 0.0f;

    std::vector<float> zcoords;
    for (int i = 0; i < zdim; i++) {
        zcoords.push_back(ord);
        ord += i * 0.1f;
    }

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
    
    cout << "Calling H5Fcreate..." << endl;
    file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    dataspace = H5Screate_simple(1, &xdim, NULL);
    dataset = H5Dcreate2(file, "/xcoords", H5T_IEEE_F32LE, dataspace,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &xcoords.front());

    status = H5Dclose(dataset);
    status = H5Sclose(dataspace);

    dataspace = H5Screate_simple(1, &ydim, NULL);
    dataset = H5Dcreate2(file, "/ycoords", H5T_IEEE_F32LE, dataspace,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &ycoords.front());

    status = H5Dclose(dataset);
    status = H5Sclose(dataspace);

    dataspace = H5Screate_simple(1, &zdim, NULL);
    dataset = H5Dcreate2(file, "/zcoords", H5T_IEEE_F32LE, dataspace,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &zcoords.front());

    status = H5Dclose(dataset);
    status = H5Sclose(dataspace);
    
    cout << "Calling H5Screate_simple..." << endl;
    dataspace = H5Screate_simple(3, dims, NULL);

    cout << "Calling H5Dcreate2..." << endl;
    dataset = H5Dcreate2(file, "/scalars", H5T_IEEE_F32LE, dataspace,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    cout << "Calling H5Dwrite..." << endl;
    status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &scalars.front());

    cout << "Closing dataset, dataspace, and file..." << endl;
    status = H5Dclose(dataset);
    status = H5Sclose(dataspace);
    status = H5Fclose(file);
    return 0;
}
