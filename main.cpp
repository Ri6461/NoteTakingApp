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

    if (file.is_open()) { // Check if the file is open
        std::string line;
        while (std::getline(file, line)) { // Read each line from the file
            notes.push_back(line); // Add the note to the vector
        }
        file.close(); // Close the file after reading
    }
    return notes;
}

// Function to save notes to a file
void saveNotes(const std::vector<std::string>& notes) {
    std::ofstream file("notes.txt");

    if (file.is_open()) { // Check if the file is open
        for (const auto& note : notes) {
            file << note << std::endl; // Write each note to the file
        }
        file.close(); // Close the file after writing
    } else {
        std::cerr << "Unable to open file for writing." << std::endl;
    }
}

// Global variables for note-taking app
std::vector<std::string> notes = loadNotes(); // Load notes from the file when the program starts
char new_note[1024] = "";  // Buffer for new note input
int selected_note = -1;     // To store the index of the selected note for editing
char edit_buffer[1024] = "";  // Buffer for editing a note's text

// Function to render the UI using Dear ImGui
void renderUI() {
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    ImGui::Begin("Note Taking App");

    // Input for a new note
    ImGui::Text("Enter a new note:");
    ImGui::InputTextMultiline("##new_note", new_note, sizeof(new_note)); // Text input field for the new note

    if (ImGui::Button("Save Note")) { // Button to save the new note
        if (strlen(new_note) > 0) {  // Check if the new note is not empty
            notes.push_back(new_note); // Add the new note to the vector
            saveNotes(notes); // Save notes to the file
            memset(new_note, 0, sizeof(new_note));  // Clear the input field after saving
        }
    }

    ImGui::Separator(); // Add a visual separator between sections
    ImGui::Text("Your Notes:");

    // Loop through all notes and display them with Edit and Delete options
    for (size_t i = 0; i < notes.size(); ++i) {
        ImGui::BulletText("%s", notes[i].c_str()); // Display each note as a bullet point

        ImGui::SameLine(); // Position the next widget on the same line
        if (ImGui::Button(("Edit##" + std::to_string(i)).c_str())) { // Edit button for the note
            selected_note = i;
            strncpy(edit_buffer, notes[i].c_str(), sizeof(edit_buffer));  // Copy note to buffer for editing
            edit_buffer[sizeof(edit_buffer) - 1] = '\0';  // Ensure the buffer is null-terminated
        }

        ImGui::SameLine();
        if (ImGui::Button(("Delete##" + std::to_string(i)).c_str())) { // Delete button for the note
            notes.erase(notes.begin() + i); // Remove the note from the vector
            saveNotes(notes); // Save the updated list of notes
            if (selected_note == static_cast<int>(i)) {
                selected_note = -1;  // Reset selected note if it was deleted
            }
        }
    }

    // Edit note popup
    if (selected_note != -1) { // If a note is selected for editing, open the popup
        ImGui::OpenPopup("Edit Note");
    }

    if (ImGui::BeginPopupModal("Edit Note", nullptr, ImGuiWindowFlags_AlwaysAutoResize)) { // Modal for editing the note
        ImGui::Text("Edit note:");
        ImGui::InputTextMultiline("##edit_note", edit_buffer, sizeof(edit_buffer), ImVec2(400, 100)); // Text input for editing

        if (ImGui::Button("Save Changes")) { // Button to save changes
            if (selected_note >= 0 && selected_note < static_cast<int>(notes.size())) {
                notes[selected_note] = std::string(edit_buffer); // Update the selected note
                saveNotes(notes); // Save the updated notes
                selected_note = -1;  // Reset the selected note
                ImGui::CloseCurrentPopup(); // Close the popup
            }
        }

        ImGui::SameLine();
        if (ImGui::Button("Cancel")) { // Button to cancel editing
            selected_note = -1; // Reset the selected note
            ImGui::CloseCurrentPopup(); // Close the popup
        }

        ImGui::EndPopup();
    }

    ImGui::End();

    // Rendering
    ImGui::Render();
    glViewport(0, 0, 1280, 720); // Set the viewport for rendering
    glClearColor(0.45f, 0.55f, 0.60f, 1.00f); // Set the clear color
    glClear(GL_COLOR_BUFFER_BIT); // Clear the color buffer
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData()); // Render Dear ImGui data
}

int main() {
    // Initialize GLFW
    if (!glfwInit()) { // Check if GLFW initialization fails
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Create window with OpenGL context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Dear ImGui Note Taking App", NULL, NULL); // Create a window
    if (!window) { // Check if the window creation fails
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate(); // Terminate GLFW
        return -1;
    }
    glfwMakeContextCurrent(window); // Make the window's OpenGL context current
    glfwSwapInterval(1);  // Enable vsync

    // Initialize OpenGL loader and Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); // Get ImGui IO object for input/output configuration
    ImGui::StyleColorsDark(); // Set the dark style for ImGui

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window, true); // Initialize ImGui for GLFW
    ImGui_ImplOpenGL3_Init("#version 130"); // Initialize ImGui for OpenGL with GLSL version

    // Main loop
    while (!glfwWindowShouldClose(window)) { // Main application loop until the window should close
        // Poll and handle events
        glfwPollEvents(); // Poll for window events

        // Render the UI
        renderUI(); // Call function to render the UI

        // Swap buffers
        glfwSwapBuffers(window); // Swap the front and back buffers
    }

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown(); // Shutdown ImGui OpenGL3 implementation
    ImGui_ImplGlfw_Shutdown(); // Shutdown ImGui GLFW implementation
    ImGui::DestroyContext(); // Destroy ImGui context
    glfwDestroyWindow(window); // Destroy the GLFW window
    glfwTerminate(); // Terminate GLFW

    return 0;
}
