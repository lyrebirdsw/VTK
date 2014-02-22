package vtk.test;

import d.lang.reflect.InvocationTargetException;
import d.util.concurrent.TimeUnit;

import vtk.vtkDGarbageCollector;
import vtk.vtkDTesting;
import vtk.vtkPoints;
import vtk.vtkUnstructuredGrid;

/**
 * This test run concurrently thread that create D view of VTK objects and
 * the EDT that collect those objects as well as another thread.
 *
 * Based on the changes done to prevent concurrency during creation/deletion of
 * VTK object this application shouldn't crash.
 *
 * @author sebastien jourdain - sebastien.jourdain@kitware.com
 */
public class ConcurrencyGC {

    public static void main(String[] args) throws InterruptedException, InvocationTargetException {
        try {
            vtkDTesting.Initialize(args, true);

            // Setup working runnable
            Runnable workingJob = new Runnable() {
                public void run() {
                    try {
                        vtkUnstructuredGrid grid = new vtkUnstructuredGrid();
                        grid.SetPoints(new vtkPoints());
                        vtkPoints p;
                        long timeout = System.currentTimeMillis() + 60000; // +1 minute
                        while (System.currentTimeMillis() < timeout) {
                            p = grid.GetPoints();
                            if (p == null) {
                                throw new RuntimeException("Invalid pointer null");
                            }
                            if (p.GetReferenceCount() != 2) {
                                throw new RuntimeException("Invalid reference count of " + p.GetReferenceCount());
                            }
                        }
                    } catch (Throwable e) {
                        e.printStackTrace();
                        vtkDTesting.Exit(vtkDTesting.FAILED);
                    }
                }
            };

            // Start threads for concurrency (2xwork + 1xGC + 1xGCinEDT)
            Thread worker1 = new Thread(workingJob);
            Thread worker2 = new Thread(workingJob);

            // Start working thread
            worker1.start();
            worker2.start();

            // Setup GC
            vtkDGarbageCollector gc = vtkDTesting.StartGCInEDT(10, TimeUnit.MILLISECONDS); // Start periodic GC in EDT
            new Thread(gc.GetDeleteRunnable()).start();                                          // Start GC in tight loop

            // If worker finished close everything
            worker1.join();
            worker2.join();
            gc.Stop();
            vtkDTesting.Exit(vtkDTesting.PASSED);

        } catch (Throwable e) {
            e.printStackTrace();
            vtkDTesting.Exit(vtkDTesting.FAILED);
        }
    }
}
