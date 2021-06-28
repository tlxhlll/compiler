#ifndef UTILS_AST
#define UTILS_AST

#include <memory>
namespace spc
{
    class BaseNode;
    template<typename T, typename U>
    inline typename std::enable_if<std::is_base_of<BaseNode, U>::value && std::is_base_of<BaseNode, T>::value, bool>::type 
    is_ptr_of(const std::shared_ptr<U> &ptr)  //判断U是不是T的指针
    {
        return dynamic_cast<T *>(ptr.get()) != nullptr;
    }

    template<typename T, typename U>
    inline typename std::enable_if<std::is_base_of<BaseNode, U>::value && std::is_base_of<BaseNode, T>::value, std::shared_ptr<T>>::type
    cast_node(const std::shared_ptr<U> &ptr)  //把类型U转换成类型T（仅当T为U的父类）
    {
        return std::dynamic_pointer_cast<T>(ptr);
    }
    
} // namespace spc
#endif
