#include "example.h"
#include "example_impl.h"

namespace mediapipe {

MP_CPP_EXPORT Example* Example::Create(const char* graph) 
{
    ExampleImpl *example = new ExampleImpl();
    absl::Status status = example->Init(graph);
    if (status.ok()){
        return example;
    }    
    else{
        LOG(INFO) << "Error initializing graph " << status.ToString();
        delete example;
        return nullptr;
    } 
}

}
