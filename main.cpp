#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>

// Function to load notes from a file
std::vector<std::string> loadNotes() {
    std::vector<std::string> notes;
    std::ifstream file("notes.txt");

    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            notes.push_back(line);
        }
        file.close();
    }
    return notes;
}

// Function to save notes to a file
void saveNotes(const std::vector<std::string>& notes) {
    std::ofstream file("notes.txt");

    if (file.is_open()) {
        for (const auto& note : notes) {
            file << note << std::endl;
        }
        file.close();
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}

// Global variables for note-taking app
std::vector<std::string> notes = loadNotes();
char new_note[1024] = "";  // Buffer for new note input
int selected_note = -1;     // For editing a specific note
char edit_buffer[1024] = "";  // Buffer for editing note text


void renderUI() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Note Taking App");

    ImGui::Text("Enter a new note:");
    ImGui::InputTextMultiline("##new_note", new_note, sizeof(new_note));

    if (ImGui::Button("Save Note")) {
        if (strlen(new_note) > 0) {  // Check if the new note is not empty
            notes.push_back(new_note);
            saveNotes(notes);
            memset(new_note, 0, sizeof(new_note));  // Clear the input field after saving the note
        }
    }

    ImGui::Separator();
    ImGui::Text("Your Notes:");

    // Display notes with Edit and Delete options
    for (size_t i = 0; i < notes.size(); ++i) {
        ImGui::BulletText("%s", notes[i].c_str());

        ImGui::SameLine();
        if (ImGui::Button(("Edit##" + std::to_string(i)).c_str())) {
            selected_note = i;
            strncpy(edit_buffer, notes[i].c_str(), sizeof(edit_buffer));  // Copy note to buffer for editing
            edit_buffer[sizeof(edit_buffer) - 1] = '\0';  // Ensure null-termination
        }

        ImGui::SameLine();
        if (ImGui::Button(("Delete##" + std::to_string(i)).c_str())) {
            notes.erase(notes.begin() + i);
            saveNotes(notes);
            if (selected_note == static_cast<int>(i)) {
                selected_note = -1;  // Reset selected note if deleted
            }
        }
    }

    // Edit note popup
    if (selected_note != -1) {
        ImGui::OpenPopup("Edit Note");
    }

    if (ImGui::BeginPopupModal("Edit Note", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) {
        ImGui::Text("Edit note:");
        ImGui::InputTextMultiline("##edit_note", edit_buffer, sizeof(edit_buffer), ImVec2(400, 100));

        if (ImGui::Button("Save Changes")) {
            if (selected_note >= 0 && selected_note < static_cast<int>(notes.size())) {
                notes[selected_note] = std::string(edit_buffer);
                saveNotes(notes);
                selected_note = -1;  // Reset the selected note after editing
                ImGui::CloseCurrentPopup();
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) {
            selected_note = -1;
            ImGui::CloseCurrentPopup();
        }

        ImGui::EndPopup();
    }

    ImGui::End();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, 1280, 720);
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f);
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create window with OpenGL context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui Note Taking App", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);  // Enable vsync

    // Initialize OpenGL loader and Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    ImGui::StyleColorsDark();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init("#version 130");

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle events
        glfwPollEvents();

        // Render the UI
        renderUI();

        // Swap buffers
        glfwSwapBuffers(window);
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
