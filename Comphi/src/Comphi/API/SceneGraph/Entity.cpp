#include "cphipch.h"
#include "Entity.h"

namespace Comphi {
    
    template<typename T>
    std::shared_ptr<T>& Entity::AddComponent(std::shared_ptr<T> componentPtr)
    {
        //sanity-Check
        static_assert(std::is_base_of<Component, T>::value, "Sub-Component not derived from BaseClass Component!");

        componentList.push_back(componentPtr);
        return &componentPtr;
    }

    template<typename T>
    T* Entity::GetComponent()
    {
        for (size_t i = 0; i < componentList.size(); i++)
        {
            //Find castable component O(N)
            T* componentPtr = dynamic_cast<T*>(componentList[i].get());
            if (componentPtr != nullptr) {
                return std::make_shared<T>(componentPtr);
            }
        }

    }

}