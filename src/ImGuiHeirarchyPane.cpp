#include "../include/ImGuiHeirarchyPane.hpp"
#include "../include/Scene.hpp"

namespace FF {
ImGuiHeirarchyPane::ImGuiHeirarchyPane(FF::Scene& scene):
  ImGuiPane("Heirarchy"), scene(scene) {
  scene.NewEntity("Entity0");
  // scene.NewEntity("Entity1");
  // scene.NewEntity("Entity2");
  // scene.NewEntity("Entity3");
  // scene.NewEntity("Entity4");
  // 
  // FF::Entity* e1 = scene.FindEntityNode("Entity1"); 
  // FF::Entity* test = e1->AddChild(scene.NewEntity("Test"));
  // FF::Entity* test2 = e1->AddChild(scene.NewEntity("Test2"));
  // test2->AddChild(scene.NewEntity("Test9"));

  // Entity *e = scene.FindEntityNode("Entity0");
  // e->AddChild(scene.NewEntity("Entity6"));
  // scene.Traverse();
}

ImGuiHeirarchyPane::~ImGuiHeirarchyPane() {}

void ImGuiHeirarchyPane::Show(FF::Window& window) {
  if (!visible)
    return;
  ImGui::Begin(name.c_str());
  if (ImGui::Button("New Entity")) {
    scene.NewEntity("Entity" + std::to_string(scene.GetEntityCount()));
  }
  ShowHeirarchy(scene.entity_tree_root);
  ImGui::End();
}

void ImGuiHeirarchyPane::ShowHeirarchy(FF::Entity* root) {
  // traverse tree and render it
  if (root == nullptr)
    return;
  if (ShowEntityNode(root)) {
    for (int i = 0; i < root->children.size(); i++) {
      ShowHeirarchy(root->children.at(i));
    }
    ImGui::TreePop();
  }
}

bool ImGuiHeirarchyPane::ShowEntityNode(FF::Entity* node) {
  std::string node_id = node->GetComponent<Identifier>().id;
  ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow;
  flags |= selection_id == node_id ? ImGuiTreeNodeFlags_Selected : 0; 

  bool opened = ImGui::TreeNodeEx(node->GetComponent<Identifier>().id.c_str(), flags);
  if (ImGui::IsItemClicked(0) && node_id != "root") {
    selection_id = node_id;
    scene.selected_entity = node;
  }
  if (ImGui::BeginDragDropSource()) {
    ImGui::SetDragDropPayload("ENTITY_MOVE_PAYLOAD", &node, sizeof(FF::Entity*));
    ImGui::EndDragDropSource();
  }
  if (ImGui::BeginDragDropTarget()) {
    if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ENTITY_MOVE_PAYLOAD")) {
      FF::Entity* e = *(FF::Entity**) payload->Data;
      if (e != node) {
        node->AddChild(e);
      }
    }
    ImGui::EndDragDropTarget();
  }
  // do other things
  return opened;
}
}
