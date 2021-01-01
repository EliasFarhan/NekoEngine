#include <editor_engine.h>
#include "model_viewer.h"
#include "editor_scene_manager.h"

#include <pybind11/embed.h> // everything needed for embedding
#include <pybind11/operators.h>

#include "editor_inspector.h"
namespace py = pybind11;

#include "editor_pyconsole.h"
#include "engine/entity.h"
#include "mathematics/matrix.h"


PYBIND11_EMBEDDED_MODULE(neko, m)
{
    py::class_<neko::EntityManager>(m, "EntityManager")
        .def("create_entity", &neko::EntityManager::CreateEntity)
        .def("destroy_entity", &neko::EntityManager::DestroyEntity);
    py::class_<neko::radian_t>(m, "radian")
        .def("value", &neko::radian_t::value);
    py::class_<neko::Vec2f>(m, "Vec2f")
        .def(py::init<>())
        .def(py::init<float>())
        .def(py::init<float, float>())
        .def_readonly_static("zero", &neko::Vec2f::zero)
        .def_readonly_static("one", &neko::Vec2f::one)
        .def_readonly_static("left", &neko::Vec2f::left)
        .def_readonly_static("right", &neko::Vec2f::right)
        .def_readonly_static("up", &neko::Vec2f::up)
        .def_readonly_static("down", &neko::Vec2f::down)
        //properties
        .def_readwrite("x", &neko::Vec2f::x)
        .def_readwrite("y", &neko::Vec2f::y)
        //operators
        .def(py::self + py::self)
        .def(py::self += py::self)
        .def(-py::self)
        .def(py::self - py::self)
        .def(py::self -= py::self)
        .def(py::self * py::self)
        .def(py::self *= float())
        .def(py::self * float())
        .def(float() * py::self)
        .def(py::self / py::self)
        .def(py::self /= float())
        .def(py::self / float())
        //vector specific
        .def("normalize", &neko::Vec2f::Normalized)
        .def_static("lerp", &neko::Vec2f::Lerp)
        .def("rotate", &neko::Vec2f::Rotate)
        .def("magnitude", &neko::Vec2f::Magnitude)
        .def("square_magnitude", &neko::Vec2f::SquareMagnitude)
        .def_static("project", &neko::Vec2f::Project)
        .def_static("dot", &neko::Vec2f::Dot)
        .def_static("angle_between", &neko::Vec2f::AngleBetween)
    ;
    py::class_<neko::Vec3f>(m, "Vec3f")
        .def(py::init<>())
        .def(py::init<float>())
        .def(py::init<float, float, float>())
        //properties
        .def_readwrite("x", &neko::Vec3f::x)
        .def_readwrite("y", &neko::Vec3f::y)
        .def_readwrite("z", &neko::Vec3f::z)
        .def_readonly_static("zero", &neko::Vec3f::zero)
        .def_readonly_static("one", &neko::Vec3f::one)
        .def_readonly_static("left", &neko::Vec3f::left)
        .def_readonly_static("right", &neko::Vec3f::right)
        .def_readonly_static("up", &neko::Vec3f::up)
        .def_readonly_static("down", &neko::Vec3f::down)
        .def_readonly_static("forward", &neko::Vec3f::forward)
        .def_readonly_static("back", &neko::Vec3f::back)
        //operators
        .def(py::self + py::self)
        .def(py::self += py::self)
        .def(-py::self)
        .def(py::self - py::self)
        .def(py::self -= py::self)
        .def(py::self * py::self)
        .def(py::self *= float())
        .def(py::self * float())
        .def(float() * py::self)
        .def(py::self /= float())
        .def(py::self / float())
        //vector specific
        .def("normalize", &neko::Vec3f::Normalized)
        .def_static("lerp", &neko::Vec3f::Lerp)
        .def_static("reflect", &neko::Vec3f::Reflect)
        .def_static("refract", &neko::Vec3f::Refract)
        .def("magnitude", &neko::Vec3f::Magnitude)
        .def("square_magnitude", &neko::Vec3f::SquareMagnitude)
        .def_static("project", &neko::Vec3f::Project)
        .def_static("dot", &neko::Vec3f::Dot)
        .def_static("angle_between", &neko::Vec3f::AngleBetween)
    ;
    py::class_<neko::Vec4f>(m, "Vec4f")
        .def(py::init<>())
        .def(py::init<float>())
        .def(py::init<float, float, float, float>())
        //properties
        .def_readwrite("x", &neko::Vec4f::x)
        .def_readwrite("y", &neko::Vec4f::y)
        .def_readwrite("z", &neko::Vec4f::z)
        .def_readwrite("z", &neko::Vec4f::w)
        //operators
        .def(py::self + py::self)
        .def(py::self += py::self)
        .def(-py::self)
        .def(py::self - py::self)
        .def(py::self -= py::self)
        .def(py::self * py::self)
        .def(py::self *= float())
        .def(py::self * float())
        .def(float() * py::self)
        .def(py::self /= float())
        .def(py::self / float())
        //vector specific
        .def("normalize", &neko::Vec4f::Normalized)
        .def_static("lerp", &neko::Vec4f::Lerp)
        .def("magnitude", &neko::Vec4f::Magnitude)
        .def("square_magnitude", &neko::Vec4f::SquareMagnitude)
        .def_static("project", &neko::Vec4f::Project)
        .def_static("dot", &neko::Vec4f::Dot)
    ;
    py::class_<neko::Mat4f>(m, "Mat4f")
        .def("transpose", &neko::Mat4f::Transpose)
        .def("inverse", &neko::Mat4f::Inverse)
        .def("determinant", &neko::Mat4f::Determinant)
        .def_readonly_static("identity", &neko::Mat4f::Identity)
        .def_readonly_static("zero", &neko::Mat4f::Zero)
        //operators
        .def(py::self + py::self)
        .def(py::self - py::self)
        .def(py::self * neko::Vec4f())
        .def(py::self * float())
        ;
}

namespace neko::editor
{

Editor::Editor()
{
    EditorLocator::provide(this);
    AddEditorSystem("PyConsole", std::make_unique<EditorPyConsole>());
    AddEditorSystem("ModelViewer", std::make_unique<ModelViewer>());
    AddEditorSystem("SceneManager", std::make_unique<EditorSceneManager>());
    AddEditorSystem("Inspector", std::make_unique<EditorInspector>());
}
void Editor::Init()
{
    for(auto& editorSystem : editorSystems_)
    {
        if(editorSystem->IsActive())
        {
            editorSystem->Init();
        }
    }
    py::initialize_interpreter();
    py::module nekoModule = py::module::import("neko");
}

void Editor::Update(seconds dt)
{
    for(auto& editorSystem : editorSystems_)
    {
        if(editorSystem->IsActive())
        {
            editorSystem->Update(dt);
        }
    }
}

void Editor::Destroy()
{
    py::finalize_interpreter();
    for(auto& editorSystem : editorSystems_)
    {
        editorSystem->Destroy();
    }
}

void Editor::DrawImGui()
{
    for(auto& editorSystem : editorSystems_)
    {
        if(editorSystem->IsActive())
        {
            editorSystem->DrawImGui();
        }
    }
}

void Editor::OnEvent(const SDL_Event& event)
{

}

void Editor::SetActiveSystem(std::string_view editorSystem, bool isActive)
{
    auto it = editorSystemsMap_.find(editorSystem.data());
    if(it != editorSystemsMap_.end())
    {
        it->second->SetActive(isActive);
    }

}

void Editor::AddEditorSystem(std::string_view editorSystemName, std::unique_ptr<EditorSystem> editorSystem)
{
    editorSystems_.push_back(std::move(editorSystem));
    editorSystemsMap_[editorSystemName.data()] = editorSystems_.back().get();
}


}
