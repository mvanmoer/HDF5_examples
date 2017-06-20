// Making a curvilinear mesh with HDF5.
// This is analogous to a vtkStructuredGrid dataset, i.e.,
// topologically regular, but geometrically arbitrary.
// The mesh coordinates will be constructed in a cylindrical 
// coordinate system, then converted to cartesian.
// There is one trivial scalar field "scalars." 
// VisIt and ParaView can read the XDMF.
#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include "hdf5.h"

int main(int argc, char** argv) {
    using std::cout;
    using std::endl;
    const double PI = atan(1.0) * 4.0;

    const std::string filename("scalar.h5");
    cout << "Output filename is: " << filename << endl;

    hid_t file; 
    hid_t dataset; 
    hid_t dataspace;

    herr_t status;

    // HDF5 convention is fastest is last, that is {zdim, ydim, xdim}.
    hsize_t rdim = 256;
    hsize_t thetadim = 192 + 1;
    hsize_t zdim = 128;
    hsize_t coorddims[4] = { zdim, thetadim, rdim, 3 };
    hsize_t scalardims[3] = { zdim, thetadim, rdim };

    std::vector<float> coords;
    std::vector<float> scalars;
    float r = 0.0f;
    float theta = 0.0f;
    float z = 0.0f;
    float dtheta = 2.0f * PI /(thetadim - 1);
    for (int k = 0; k < coorddims[0]; k++) {
        for (int j = 0; j < coorddims[1]; j++) {
            for (int i = 0; i < coorddims[2]; i++) {
                float x = r * cos(theta);
                float y = r * sin(theta);
                coords.push_back(x);
                coords.push_back(y);
                coords.push_back(z);
                r += 0.1f;
                scalars.push_back(sin(theta) * cos(theta));
            }
            r = 0.0f;
            theta += dtheta; 
        }
        theta = 0.0f;
        z += 0.1f;
    }
    
    cout << "Calling H5Fcreate..." << endl;
    file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    cout << "Calling H5Screate_simple for coords..." << endl;
    dataspace = H5Screate_simple(4, coorddims, NULL);

    cout << "Calling H5Dcreate2 for coords..." << endl;
    dataset = H5Dcreate2(file, "/coords", H5T_IEEE_F32LE, dataspace,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    cout << "Calling H5Dwrite for coords..." << endl;
    status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &coords.front());

    cout << "Closing dataset and dataspace for coords..." << endl;
    status = H5Dclose(dataset);
    status = H5Sclose(dataspace);
    
    cout << "Calling H5Screate_simple for scalars..." << endl;
    dataspace = H5Screate_simple(3, scalardims, NULL);

    cout << "Calling H5Dcreate2 for scalars..." << endl;
    dataset = H5Dcreate2(file, "/scalars", H5T_IEEE_F32LE, dataspace,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    cout << "Calling H5Dwrite for scalars..." << endl;
    status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &scalars.front());

    cout << "Closing dataset and dataspace for scalars..." << endl;
    status = H5Dclose(dataset);
    status = H5Sclose(dataspace);

    cout << "Closing file..." << endl;
    status = H5Fclose(file);
    return 0;
}
