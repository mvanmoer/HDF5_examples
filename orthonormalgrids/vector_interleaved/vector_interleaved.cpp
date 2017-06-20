// Makng an orthonormal mesh with HDF5.
// This is analogous to a vtkImageData dataset.
// There is one trivial vector field "vectors." 
// The vector components are interleaved in a single
// std::vector<float>.
// VisIt/ParaView will not load the .h5, but will need to be
// given vector_interleaved.xdmf, which is hand-written.

#include <string>
#include <iostream>
#include <vector>
#include <cmath>
#include "hdf5.h"

int main(int argc, char** argv) {
    using std::cout;
    using std::endl;

    const std::string filename("vector_interleaved.h5");
    cout << "Output filename is: " << filename << endl;

    hid_t file; 
    hid_t dataset; 
    hid_t dataspace;

    herr_t status;

    // HDF5 convention is fastest is last, this is {zdim, ydim, xdim, numcomps}.
    hsize_t dims[4] = { 128, 192, 256, 3 };
    
    // arbitrary vector data for writing.
    cout << "Creating vector data..." << endl;
    std::vector<float> vectors;
    for (int k = 0; k < dims[0]; k++) {
        for (int j = 0; j < dims[1]; j++) {
            for (int i = 0; i < dims[2]; i++) {
                vectors.push_back(1.0f/(i+1));
                vectors.push_back(1.0f/(j+1));
                vectors.push_back(1.0f/(k+1));
            }
        }
    }

    cout << "Calling H5Fcreate..." << endl;    
    file = H5Fcreate(filename.c_str(), H5F_ACC_TRUNC, H5P_DEFAULT, H5P_DEFAULT);

    cout << "Calling H5Screate_simple..." << endl;
    dataspace = H5Screate_simple(4, dims, NULL);
    
    cout << "Calling H5Dcreate2..." << endl; 
    dataset = H5Dcreate2(file, "/vectors", H5T_IEEE_F32LE, dataspace,
            H5P_DEFAULT, H5P_DEFAULT, H5P_DEFAULT);

    cout << "Calling H5Dwrite..." << endl;
    status = H5Dwrite(dataset, H5T_NATIVE_FLOAT, H5S_ALL, H5S_ALL, H5P_DEFAULT, &vectors.front());

    cout << "Closing dataspace..." << endl;
    status = H5Sclose(dataspace);

    cout << "Closing dataset..." << endl;
    status = H5Dclose(dataset);

    cout << "Closing file..." << endl;
    status = H5Fclose(file);
    return 0;
}
