package vtk.test;

import vtk.vtkDoubleArray;
import vtk.vtkDTesting;
import vtk.vtkObject;
import vtk.vtkQuadric;
import vtk.vtkSampleFunction;

/**
 * This test should go on forever without crashing.
 */
public class DDelete {

    public static void main(String[] args) {
        try {
            vtkDTesting.Initialize(args, true);

            // Start working code
            long timeout = System.currentTimeMillis() + 60000; // +1 minute
            while (System.currentTimeMillis() < timeout) {
                vtkDoubleArray arr = new vtkDoubleArray();
                arr.Delete();

                vtkQuadric quadric = new vtkQuadric();
                vtkSampleFunction sample = new vtkSampleFunction();
                sample.SetSampleDimensions(30, 30, 30);
                sample.SetImplicitFunction(quadric);
                sample.GetImplicitFunction();
                sample.Delete();
                quadric.Delete();

                // Make sure the D VTK object map is empty
                if (vtkObject.D_OBJECT_MANAGER.getSize() > 1) { // vtkTesting
                    System.out.println(vtkObject.D_OBJECT_MANAGER.gc(true).listKeptReferenceToString());
                    throw new RuntimeException("There shouldn't have any VTK object inside the map as we are using Delete().");
                }
            }

            vtkDTesting.Exit(vtkDTesting.PASSED);
        } catch (Throwable e) {
            e.printStackTrace();
            vtkDTesting.Exit(vtkDTesting.FAILED);
        }
    }
}
