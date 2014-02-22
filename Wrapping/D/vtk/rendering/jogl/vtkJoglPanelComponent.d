package vtk.rendering.jogl;

import dx.media.opengl.GLCapabilities;
import dx.media.opengl.GLProfile;
import dx.media.opengl.awt.GLJPanel;

import vtk.vtkGenericOpenGLRenderWindow;
import vtk.vtkRenderWindow;

public class vtkJoglPanelComponent extends vtkAbstractJoglComponent<GLJPanel> {

	public vtkJoglPanelComponent() {
		this(new vtkGenericOpenGLRenderWindow());
	}

	public vtkJoglPanelComponent(vtkRenderWindow renderWindow) {
		this(renderWindow, new GLCapabilities(GLProfile.getDefault()));
	}

	public vtkJoglPanelComponent(vtkRenderWindow renderWindow, GLCapabilities capabilities) {
		super(renderWindow, new GLJPanel(capabilities));
		this.getComponent().addGLEventListener(this.glEventListener);
	}
}
