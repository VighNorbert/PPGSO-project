#ifndef PPGSO_ORIGINWINDOW_HPP
#define PPGSO_ORIGINWINDOW_HPP


class OriginWindow : public ppgso::Window {
private:

    glm::vec3 viewRotation{0,0,0};
    glm::vec3 viewTranslation{0,0,0};
public:
    OriginWindow(int SIZE_X, int SIZE_Y) : Window{"Demoscéna - Mesto", SIZE_X, SIZE_Y} {

    }

    void onIdle() override {
        // Set gray background
        glClearColor(.1f,.1f,.1f,1.0f);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    }
};


#endif //PPGSO_ORIGINWINDOW_HPP
