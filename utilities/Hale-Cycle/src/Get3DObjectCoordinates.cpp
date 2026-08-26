void Get3DObjectCoordinates(int mouseX, int mouseY, double& objX, double& objY, double& objZ) {
    GLdouble modelview[16];
    GLdouble projection[16];
    GLint viewport[4];
    GLfloat winZ;

    // 1. Get the current matrices and viewport
    glGetDoublev(GL_MODELVIEW_MATRIX, modelview);
    glGetDoublev(GL_PROJECTION_MATRIX, projection);
    glGetIntegerv(GL_VIEWPORT, viewport);

    // 2. Transform mouse y-coordinate (window system origin is top-left, OpenGL is bottom-left)
    int winX = mouseX;
    int winY = viewport[3] - mouseY;

    // 3. Read depth value from depth buffer under the cursor
    glReadPixels(winX, winY, 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ);

    // 4. Map screen coordinates to 3D object space coordinates
    gluUnProject((GLdouble)winX, (GLdouble)winY, (GLdouble)winZ, 
                 modelview, projection, viewport, 
                 &objX, &objY, &objZ);
}
