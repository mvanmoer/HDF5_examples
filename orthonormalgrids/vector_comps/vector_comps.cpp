// Makng an orthonormal mesh with HDF5.
// This is analogous to a vtkImageData dataset.
// There is one trivial vector field "vectors." 
// The vector components are packed into three separate
// std::vector<float>'s which are then written as if
// they were individual scalars. An XDMF join operation
// will combine them.
// ParaView will not load the .h5, but will need to be
// given vector_comps.xdmf, which is hand-written.
// VisIt will load the xdmf, but doesn't appear to understand
// the it.

#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include "hdf5.h"

int main(int argc, char** argv) {
    using std::cout;
    using std::endl;

    const std::string filename("vector_comps.h5");
    cout << "Output filename is: " << filename << endl;

    hid_t file; 
    hid_t dataset; 
    hid_t dataspace;

    herr_t status;

    // HDF5 convention is fastest is last, this is {zdim, ydim, xdim}.
    hsize_t dims[3] = { 128, 192, 256 };
    
    // arbitrary vector data for writing.
    cout << "Creating the vector data..." << endl;
    std::vector<float> xcomps;
    std::vector<float> ycomps;
    std::vector<float> zcomps;
    for (int k = 0; k < dims[0]; k++) {
        for (int j = 0; j < dims[1]; j++) {
            for (int i = 0; i < dims[2]; i++) {
                xcomps.push_back(1.0f/(i+1));
                ycomps.push_back(1.0f/(j+1));
                zcomps.push_back(1.0f/(k+1));
            }
        }
    }
   
    cout << "Calling H5Fcreate..." << endl;
    file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    cout << "Calling H5Screate_simple for the x components..." << endl;
    dataspace = H5Screate_simple(3, dims, NULL);

    cout << "Calling H5Dcreate2 for the x components..." << endl;
    dataset = H5Dcreate2(file, "/xcomps", H5T_IEEE_F32LE, dataspace,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    cout << "Calling H5Dwrite for the x components..." << endl;
    status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &xcomps.front());
    
    cout << "Closing the x component dataspace..." << endl;
    status = H5Sclose(dataspace);

    cout << "Closing the x component dataset..." << endl;
    status = H5Dclose(dataset);

    cout << "Calling H5Screate_simple for the y components..." << endl;
    dataspace = H5Screate_simple(3, dims, NULL);
     
    cout << "Calling H5Dcreate2 for the y components..." << endl;
    dataset = H5Dcreate2(file, "/ycomps", H5T_IEEE_F32LE, dataspace,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    cout << "Calling H5Dwrite for the y components..." << endl;
    status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &ycomps.front());

    cout << "Closing the y component dataspace..." << endl;
    status = H5Sclose(dataspace);

    cout << "Closing the y component dataset..." << endl;
    status = H5Dclose(dataset);
    
    cout << "Calling H5Screate_simple for the z components..." << endl;
    dataspace = H5Screate_simple(3, dims, NULL);
     
    cout << "Calling H5Dcreate2 for the z components..." << endl;
    dataset = H5Dcreate2(file, "/zcomps", H5T_IEEE_F32LE, dataspace,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    cout << "Calling H5Dwrite for the z components..." << endl;
    status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &zcomps.front());

    cout << "Closing the z component dataspace..." << endl;
    status = H5Sclose(dataspace);

    cout << "Closing the z component dataset..." << endl;
    status = H5Dclose(dataset);

    cout << "Closing the file..." << endl;
    status = H5Fclose(file);
    return 0;
}


