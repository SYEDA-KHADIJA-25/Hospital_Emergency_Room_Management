#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>

#include "Patient.h"
#include "Doctor.h"
#include "Resource.h"
#include "FileManager.h"
#include "Minheap.h"
#include "BST.h"

// ============================================================================
// STYLING AND UTILITY FUNCTIONS
// ============================================================================

void SetupModernStyle() {
    ImGuiStyle& style = ImGui::GetStyle();
    
    // Rounded corners
    style.WindowRounding = 0.0f;
    style.ChildRounding = 8.0f;
    style.FrameRounding = 6.0f;
    style.PopupRounding = 6.0f;
    style.ScrollbarRounding = 8.0f;
    style.GrabRounding = 6.0f;
    style.TabRounding = 6.0f;
    
    // Padding and spacing
    style.WindowPadding = ImVec2(15, 15);
    style.FramePadding = ImVec2(12, 8);
    style.ItemSpacing = ImVec2(12, 8);
    style.ItemInnerSpacing = ImVec2(8, 6);
    style.ScrollbarSize = 14.0f;
    
    // Professional color scheme
    ImVec4* colors = style.Colors;
    
    // Background colors
    colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_ChildBg] = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_PopupBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.98f);
    
    // Border
    colors[ImGuiCol_Border] = ImVec4(0.85f, 0.87f, 0.89f, 1.00f);
    
    // Frame (inputs, etc)
    colors[ImGuiCol_FrameBg] = ImVec4(0.98f, 0.98f, 0.99f, 1.00f);
    colors[ImGuiCol_FrameBgHovered] = ImVec4(0.95f, 0.96f, 0.98f, 1.00f);
    colors[ImGuiCol_FrameBgActive] = ImVec4(0.92f, 0.94f, 0.96f, 1.00f);
    
    // Title
    colors[ImGuiCol_TitleBg] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TitleBgActive] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.26f, 0.59f, 0.98f, 0.75f);
    
    // Buttons
    colors[ImGuiCol_Button] = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
    colors[ImGuiCol_ButtonHovered] = ImVec4(0.31f, 0.64f, 1.00f, 1.00f);
    colors[ImGuiCol_ButtonActive] = ImVec4(0.21f, 0.54f, 0.93f, 1.00f);
    
    // Headers
    colors[ImGuiCol_Header] = ImVec4(0.26f, 0.59f, 0.98f, 0.25f);
    colors[ImGuiCol_HeaderHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
    colors[ImGuiCol_HeaderActive] = ImVec4(0.26f, 0.59f, 0.98f, 0.55f);
    
    // Text
    colors[ImGuiCol_Text] = ImVec4(0.15f, 0.20f, 0.25f, 1.00f);
    colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
}

// Draw a professional stat card
void DrawStatCard(const char* title, const char* value, const char* subtitle, ImVec4 accentColor, float width = 240.0f) {
    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
    ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 10.0f);
    ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);
    
    ImGui::BeginChild(title, ImVec2(width, 130), true, ImGuiWindowFlags_NoScrollbar);
    
    // Accent bar on left
    ImDrawList* draw_list = ImGui::GetWindowDrawList();
    ImVec2 p = ImGui::GetCursorScreenPos();
    draw_list->AddRectFilled(ImVec2(p.x - 15, p.y - 15), ImVec2(p.x - 11, p.y + 145), ImGui::ColorConvertFloat4ToU32(accentColor));
    
    ImGui::Spacing();
    
    // Title
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.55f, 0.60f, 1.0f));
    ImGui::Text("%s", title);
    ImGui::PopStyleColor();
    
    ImGui::Spacing();
    ImGui::Spacing();
    
    // Value (large)
    ImGui::PushStyleColor(ImGuiCol_Text, accentColor);
    ImGui::SetWindowFontScale(2.5f);
    ImGui::Text("%s", value);
    ImGui::SetWindowFontScale(1.0f);
    ImGui::PopStyleColor();
    
    ImGui::Spacing();
    
    // Subtitle
    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.60f, 0.65f, 0.70f, 1.0f));
    ImGui::TextWrapped("%s", subtitle);
    ImGui::PopStyleColor();
    
    ImGui::EndChild();
    ImGui::PopStyleVar(2);
    ImGui::PopStyleColor();
}

// Draw colored priority badge
void DrawPriorityBadge(Priority priority) {
    ImVec4 bgColor, textColor;
    const char* text;
    
    switch (priority) {
        case Priority::High:
            bgColor = ImVec4(1.0f, 0.92f, 0.93f, 1.0f);
            textColor = ImVec4(0.85f, 0.15f, 0.15f, 1.0f);
            text = "CRITICAL";
            break;
        case Priority::Medium:
            bgColor = ImVec4(1.0f, 0.95f, 0.88f, 1.0f);
            textColor = ImVec4(0.95f, 0.50f, 0.0f, 1.0f);
            text = "URGENT";
            break;
        default:
            bgColor = ImVec4(0.91f, 0.96f, 0.91f, 1.0f);
            textColor = ImVec4(0.25f, 0.65f, 0.30f, 1.0f);
            text = "STANDARD";
            break;
    }
    
    ImGui::PushStyleColor(ImGuiCol_Button, bgColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, bgColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, bgColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::Button(text, ImVec2(95, 0));
    ImGui::PopStyleColor(4);
}

// Draw status badge
void DrawStatusBadge(const std::string& status) {
    ImVec4 bgColor, textColor;
    
    if (status == "Available") {
        bgColor = ImVec4(0.91f, 0.96f, 0.91f, 1.0f);
        textColor = ImVec4(0.25f, 0.65f, 0.30f, 1.0f);
    } else {
        bgColor = ImVec4(1.0f, 0.92f, 0.93f, 1.0f);
        textColor = ImVec4(0.85f, 0.15f, 0.15f, 1.0f);
    }
    
    ImGui::PushStyleColor(ImGuiCol_Button, bgColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, bgColor);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, bgColor);
    ImGui::PushStyleColor(ImGuiCol_Text, textColor);
    ImGui::Button(status.c_str(), ImVec2(90, 0));
    ImGui::PopStyleColor(4);
}

// ============================================================================
// APPLICATION STATE
// ============================================================================

enum class Page {
    Dashboard,
    PatientRegistration,
    PatientQueue,
    EmergencyAlerts,
    Doctors,
    Resources
};

// ============================================================================
// MAIN FUNCTION
// ============================================================================

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return -1;
    }

    const char* glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(1600, 900, "GlobalCare Hospital - Emergency Management System", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Setup ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;

    SetupModernStyle();

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Load data
    std::vector<Patient> patients = FileManager::loadPatients("data/patients.csv");
    std::vector<Doctor> doctors = FileManager::loadDoctors("data/doctors.csv");
    std::vector<Resource> resources = FileManager::loadResources("data/resources.csv");
    
    // Triage system
    Triage triage;
    for (const auto& p : patients) {
        triage.push(p);
    }

    // Application state
    Page currentPage = Page::Dashboard;
    
    // Form data for patient registration
    char patientID[64] = "";
    char patientName[128] = "";
    char patientAge[16] = "";
    char patientCondition[256] = "";
    int priorityIndex = 0;
    const char* priorities[] = { "Standard", "Urgent", "Critical" };

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        int display_w, display_h;
        glfwGetFramebufferSize(window, &display_w, &display_h);

        // ====================================================================
        // HEADER BAR
        // ====================================================================
        ImGui::SetNextWindowPos(ImVec2(0, 0));
        ImGui::SetNextWindowSize(ImVec2((float)display_w, 85));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("Header", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
        
        ImGui::SetCursorPosY(15);
        ImGui::Indent(25);
        
        // Hospital icon and title
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 0.2f));
        ImGui::Button("+", ImVec2(55, 55));
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        ImGui::SetCursorPosY(20);
        ImGui::BeginGroup();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::SetWindowFontScale(1.4f);
        ImGui::Text("GlobalCare Hospital");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::SetWindowFontScale(0.9f);
        ImGui::Text("Emergency Management System");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopStyleColor();
        ImGui::EndGroup();
        
        // User info on right
        ImGui::SameLine(ImGui::GetWindowWidth() - 280);
        ImGui::SetCursorPosY(15);
        ImGui::BeginGroup();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.9f));
        ImGui::Text("Logged in as");
        ImGui::PopStyleColor();
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::SetWindowFontScale(1.1f);
        ImGui::Text("Dr. Smith");
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopStyleColor();
        ImGui::EndGroup();
        
        ImGui::SameLine();
        ImGui::SetCursorPosY(15);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
        ImGui::Button("DS", ImVec2(55, 55));
        ImGui::PopStyleColor(2);
        
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        // ====================================================================
        // SIDEBAR
        // ====================================================================
        ImGui::SetNextWindowPos(ImVec2(0, 85));
        ImGui::SetNextWindowSize(ImVec2(280, (float)(display_h - 85)));
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.97f, 0.98f, 0.99f, 1.0f));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("Sidebar", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar);
        
        ImGui::Dummy(ImVec2(0, 15));
        
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
        ImGui::Text("   NAVIGATION");
        ImGui::PopStyleColor();
        
        ImGui::Dummy(ImVec2(0, 10));
        
        // Navigation buttons
        auto NavButton = [&](const char* label, Page page) {
            bool isActive = (currentPage == page);
            
            if (isActive) {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 0.15f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
            } else {
                ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.0f, 0.0f, 0.0f, 0.0f));
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.40f, 0.45f, 0.50f, 1.0f));
            }
            
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.26f, 0.59f, 0.98f, 0.10f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.26f, 0.59f, 0.98f, 0.20f));
            
            bool clicked = ImGui::Button(label, ImVec2(260, 45));
            
            ImGui::PopStyleColor(4);
            
            if (clicked) currentPage = page;
        };
        
        NavButton("   Dashboard", Page::Dashboard);
        NavButton("   + Patient Registration", Page::PatientRegistration);
        NavButton("   Patient Queue", Page::PatientQueue);
        NavButton("   ! Emergency Alerts", Page::EmergencyAlerts);
        NavButton("   Doctors", Page::Doctors);
        NavButton("   Resources", Page::Resources);
        
        ImGui::Dummy(ImVec2(0, 20));
        ImGui::Separator();
        ImGui::Dummy(ImVec2(0, 10));
        
        // Statistics section
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
        ImGui::Text("   STATISTICS");
        ImGui::PopStyleColor();
        
        ImGui::Dummy(ImVec2(0, 10));
        
        ImGui::Indent(20);
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.35f, 0.40f, 0.45f, 1.0f));
        ImGui::BulletText("Total Patients");
        ImGui::SameLine(200);
        ImGui::Text("%d", (int)patients.size());
        
        int criticalCount = 0;
        for (const auto& p : patients) {
            if (p.getPriority() == Priority::High) criticalCount++;
        }
        ImGui::BulletText("Critical");
        ImGui::SameLine(200);
        ImGui::TextColored(ImVec4(0.95f, 0.30f, 0.30f, 1.0f), "%d", criticalCount);
        ImGui::PopStyleColor();
        ImGui::Unindent(20);
        
        ImGui::End();
        ImGui::PopStyleVar();
        ImGui::PopStyleColor();

        // ====================================================================
        // MAIN CONTENT AREA
        // ====================================================================
        ImGui::SetNextWindowPos(ImVec2(280, 85));
        ImGui::SetNextWindowSize(ImVec2((float)(display_w - 280), (float)(display_h - 85)));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::Begin("Content", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

        // ====================================================================
        // DASHBOARD PAGE
        // ====================================================================
        if (currentPage == Page::Dashboard) {
            ImGui::SetWindowFontScale(1.8f);
            ImGui::Text("Welcome, Dr. Smith!");
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
            ImGui::Text("Emergency Management Dashboard - Real-time Overview");
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 25));
            
            // Stats cards
            int totalPatients = patients.size();
            int criticalCases = 0, urgentCases = 0, standardCases = 0;
            
            for (const auto& p : patients) {
                switch (p.getPriority()) {
                    case Priority::High: criticalCases++; break;
                    case Priority::Medium: urgentCases++; break;
                    case Priority::Low: standardCases++; break;
                }
            }
            
            char buf[32];
            
            // First row
            sprintf(buf, "%d", totalPatients);
            DrawStatCard("Total Patients Waiting", buf, "In emergency queue", ImVec4(0.26f, 0.59f, 0.98f, 1.0f), 260);
            
            ImGui::SameLine();
            
            const Patient* nextPatient = nullptr;
            if (!triage.isEmpty()) {
                nextPatient = &triage.getHeap()[0];
            }
            
            DrawStatCard("Next Patient", nextPatient ? nextPatient->getName().c_str() : "None", 
                        nextPatient ? "Highest priority" : "Queue empty", 
                        ImVec4(0.30f, 0.80f, 0.60f, 1.0f), 260);
            
            ImGui::SameLine();
            
            sprintf(buf, "%d", criticalCases);
            DrawStatCard("Critical Cases", buf, "Immediate attention required", ImVec4(0.95f, 0.30f, 0.30f, 1.0f), 260);
            
            ImGui::Dummy(ImVec2(0, 15));
            
            // Second row
            sprintf(buf, "%d", urgentCases);
            DrawStatCard("Urgent Cases", buf, "Urgent care needed", ImVec4(1.0f, 0.60f, 0.0f, 1.0f), 260);
            
            ImGui::SameLine();
            
            sprintf(buf, "%d", standardCases);
            DrawStatCard("Standard Cases", buf, "Normal priority", ImVec4(0.40f, 0.70f, 0.40f, 1.0f), 260);
            
            ImGui::SameLine();
            
            DrawStatCard("Avg. Wait Time", "--- min", "Estimated", ImVec4(0.60f, 0.40f, 0.90f, 1.0f), 260);
            
            ImGui::Dummy(ImVec2(0, 25));
            
            // Activity Graph placeholder
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::BeginChild("Graph", ImVec2(0, 250), true);
            ImGui::SetWindowFontScale(1.2f);
            ImGui::Text("Patient Activity Graph");
            ImGui::SetWindowFontScale(1.0f);
            ImGui::Dummy(ImVec2(0, 10));
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.60f, 0.65f, 0.70f, 1.0f));
            ImGui::TextWrapped("Real-time visualization of patient admissions and treatment progress would appear here.");
            ImGui::PopStyleColor();
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }

        // ====================================================================
        // PATIENT REGISTRATION PAGE
        // ====================================================================
        else if (currentPage == Page::PatientRegistration) {
            ImGui::SetWindowFontScale(1.6f);
            ImGui::Text("Patient Registration");
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
            ImGui::Text("Register a new patient in the emergency system");
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 25));
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::BeginChild("RegistrationForm", ImVec2(850, 0), true);
            
            ImGui::Dummy(ImVec2(0, 10));
            
            ImGui::Text("Patient Information");
            ImGui::Separator();
            ImGui::Dummy(ImVec2(0, 15));
            
            ImGui::Columns(2, nullptr, false);
            ImGui::SetColumnWidth(0, 400);
            
            // Left column
            ImGui::Text("Patient ID");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##patientid", patientID, IM_ARRAYSIZE(patientID));
            
            ImGui::Dummy(ImVec2(0, 10));
            
            ImGui::Text("Full Name");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##patientname", patientName, IM_ARRAYSIZE(patientName));
            
            ImGui::Dummy(ImVec2(0, 10));
            
            ImGui::Text("Age");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputText("##patientage", patientAge, IM_ARRAYSIZE(patientAge));
            
            ImGui::NextColumn();
            
            // Right column
            ImGui::Text("Priority Level");
            ImGui::SetNextItemWidth(-1);
            ImGui::Combo("##priority", &priorityIndex, priorities, IM_ARRAYSIZE(priorities));
            
            ImGui::Dummy(ImVec2(0, 10));
            
            ImGui::Text("Medical Condition");
            ImGui::SetNextItemWidth(-1);
            ImGui::InputTextMultiline("##condition", patientCondition, IM_ARRAYSIZE(patientCondition), ImVec2(-1, 120));
            
            ImGui::Columns(1);
            
            ImGui::Dummy(ImVec2(0, 20));
            
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.26f, 0.59f, 0.98f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.31f, 0.64f, 1.00f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.21f, 0.54f, 0.93f, 1.0f));
            
            if (ImGui::Button("Register Patient", ImVec2(200, 45))) {
                Priority priority = Priority::Low;
                if (priorityIndex == 1) priority = Priority::Medium;
                if (priorityIndex == 2) priority = Priority::High;
                
                int age = atoi(patientAge);
                
                Patient newPatient(patientID, patientName, age, patientCondition, priority);
                patients.push_back(newPatient);
                triage.push(newPatient);
                
                FileManager::savePatients("data/patients.csv", patients);
                
                // Clear form
                patientID[0] = '\0';
                patientName[0] = '\0';
                patientAge[0] = '\0';
                patientCondition[0] = '\0';
                priorityIndex = 0;
            }
            
            ImGui::PopStyleColor(3);
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }

        // ====================================================================
        // PATIENT QUEUE PAGE
        // ====================================================================
        else if (currentPage == Page::PatientQueue) {
            ImGui::SetWindowFontScale(1.6f);
            ImGui::Text("Patient Queue");
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
            ImGui::Text("All patients sorted by priority");
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 25));
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::BeginChild("QueueTable", ImVec2(0, 0), true);
            
            if (ImGui::BeginTable("PatientTable", 5, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp)) {
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("Patient ID", ImGuiTableColumnFlags_WidthFixed, 120);
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 200);
                ImGui::TableSetupColumn("Age", ImGuiTableColumnFlags_WidthFixed, 80);
                ImGui::TableSetupColumn("Priority", ImGuiTableColumnFlags_WidthFixed, 130);
                ImGui::TableSetupColumn("Condition", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableHeadersRow();
                
                // Sort by priority
                std::vector<Patient> sortedPatients = patients;
                std::sort(sortedPatients.begin(), sortedPatients.end(), [](const Patient& a, const Patient& b) {
                    return static_cast<int>(a.getPriority()) > static_cast<int>(b.getPriority());
                });
                
                for (const auto& patient : sortedPatients) {
                    ImGui::TableNextRow();
                    
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", patient.getID().c_str());
                    
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", patient.getName().c_str());
                    
                    ImGui::TableNextColumn();
                    ImGui::Text("%d", patient.getAge());
                    
                    ImGui::TableNextColumn();
                    DrawPriorityBadge(patient.getPriority());
                    
                    ImGui::TableNextColumn();
                    ImGui::TextWrapped("%s", patient.getCondition().c_str());
                }
                
                ImGui::EndTable();
            }
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }

        // ====================================================================
        // EMERGENCY ALERTS PAGE
        // ====================================================================
        else if (currentPage == Page::EmergencyAlerts) {
            ImGui::SetWindowFontScale(1.6f);
            ImGui::Text("Emergency Alerts");
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
            ImGui::Text("Critical and urgent cases requiring immediate attention");
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 25));
            
            for (const auto& p : patients) {
                if (p.getPriority() == Priority::High || p.getPriority() == Priority::Medium) {
                    ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
                    ImGui::BeginChild(p.getID().c_str(), ImVec2(0, 110), true);
                    
                    ImGui::Columns(2, nullptr, false);
                    ImGui::SetColumnWidth(0, 150);
                    
                    DrawPriorityBadge(p.getPriority());
                    
                    ImGui::NextColumn();
                    
                    ImGui::SetWindowFontScale(1.2f);
                    ImGui::Text("%s", p.getName().c_str());
                    ImGui::SetWindowFontScale(1.0f);
                    
                    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
                    ImGui::Text("ID: %s | Age: %d", p.getID().c_str(), p.getAge());
                    ImGui::PopStyleColor();
                    
                    ImGui::Spacing();
                    ImGui::TextWrapped("Condition: %s", p.getCondition().c_str());
                    
                    ImGui::Columns(1);
                    
                    ImGui::EndChild();
                    ImGui::PopStyleColor();
                    
                    ImGui::Dummy(ImVec2(0, 10));
                }
            }
        }

        // ====================================================================
        // DOCTORS PAGE
        // ====================================================================
        else if (currentPage == Page::Doctors) {
            ImGui::SetWindowFontScale(1.6f);
            ImGui::Text("Medical Staff");
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
            ImGui::Text("Available doctors and their current status");
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 25));
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::BeginChild("DoctorsTable", ImVec2(0, 0), true);
            
            if (ImGui::BeginTable("DoctorTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp)) {
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("Doctor ID", ImGuiTableColumnFlags_WidthFixed, 120);
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 250);
                ImGui::TableSetupColumn("Specialty", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 120);
                ImGui::TableHeadersRow();
                
                for (const auto& doctor : doctors) {
                    ImGui::TableNextRow();
                    
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", doctor.getID().c_str());
                    
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", doctor.getName().c_str());
                    
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", doctor.getSpecialty().c_str());
                    
                    ImGui::TableNextColumn();
                    DrawStatusBadge(doctor.getStatus());
                }
                
                ImGui::EndTable();
            }
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }

        // ====================================================================
        // RESOURCES PAGE
        // ====================================================================
        else if (currentPage == Page::Resources) {
            ImGui::SetWindowFontScale(1.6f);
            ImGui::Text("Hospital Resources");
            ImGui::SetWindowFontScale(1.0f);
            
            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.50f, 0.55f, 0.60f, 1.0f));
            ImGui::Text("Available equipment and facilities");
            ImGui::PopStyleColor();
            
            ImGui::Dummy(ImVec2(0, 25));
            
            ImGui::PushStyleColor(ImGuiCol_ChildBg, ImVec4(1.0f, 1.0f, 1.0f, 1.0f));
            ImGui::BeginChild("ResourcesTable", ImVec2(0, 0), true);
            
            if (ImGui::BeginTable("ResourceTable", 4, ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_ScrollY | ImGuiTableFlags_SizingStretchProp)) {
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableSetupColumn("Resource ID", ImGuiTableColumnFlags_WidthFixed, 120);
                ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_WidthFixed, 250);
                ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthStretch);
                ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 120);
                ImGui::TableHeadersRow();
                
                for (const auto& resource : resources) {
                    ImGui::TableNextRow();
                    
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", resource.getID().c_str());
                    
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", resource.getName().c_str());
                    
                    ImGui::TableNextColumn();
                    ImGui::Text("%s", resource.getType().c_str());
                    
                    ImGui::TableNextColumn();
                    DrawStatusBadge(resource.getStatus());
                }
                
                ImGui::EndTable();
            }
            
            ImGui::EndChild();
            ImGui::PopStyleColor();
        }

        ImGui::End();
        ImGui::PopStyleVar();

        // Rendering
        ImGui::Render();
        glViewport(0, 0, display_w, display_h);
        glClearColor(0.95f, 0.96f, 0.98f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

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