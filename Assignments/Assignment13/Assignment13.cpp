// Following the Vulkan Tutorial as shown in the enclose Assignment13.pdf, complete
// this Vulkan initialization sample. You can copy and past code from Example E08,
// or from other assginments such as Assignment0.cpp

#define GLFW_INCLUDE_VULKAN

#define VK_USE_PLATFORM_WIN32_KHR
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>


#include <GLFW/glfw3.h>

#include <iostream>
#include <stdexcept>
#include <cstdlib>
#include <vector>
#include <optional>
#include <set>

const uint32_t WIDTH = 640;
const uint32_t HEIGHT = 480;

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() {
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};

class Assignment13 {
public:
    void run() {
        initWindow();
        initVulkan();
        initApp();
        mainLoop();
        cleanup();
    }

private:
	// place all the application variables here. As an example, here the one for the GLFW Window and the Vulkan instances are already defined.
	
    //the Windows is the part of screen where visualize all the information
    GLFWwindow* window;
    //The instance is the connection between your application and the Vulkan library
    VkInstance instance;
    //phisical device to use
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    //logical device is used to interface with the physical one
    VkDevice device;
    // Device graphics queue
    VkQueue graphicsQueue;
    //Surface
    VkSurfaceKHR surface;
    //presentation queue
    VkQueue presentQueue;
    //command buffer
    VkCommandPool commandPool;
    //command buffer
    VkCommandBuffer commandBuffer;

    void initWindow() {
        //inizialize GLFW
        glfwInit();

        //WINDOWS CREATION
        //Defalut windows -> OPENGL, we need this to set the windows for Vulkan  
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
        //define the size and the name of the windows
        window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 13", nullptr, nullptr);
    }

    void initVulkan() {
        //creation of an instance
		createInstance();
        // Prsentation Surface Creation
        createSurface();
        // Physical Device selection
        pickPhysicalDevice();
        // Logical Device creation
        createLogicalDevice();
        // Command Pool creation
        createCommandPool();
        // Command Buffer creation
        createCommandBuffer();
    }
    //POINT 2 OF THE ASSIGNMENT - create an instance
    //Instances allows each library to work independently from the others
    void createInstance() {
        //fill in a struct with some information about our application. This data is technically optional
        //in order to create an instance list of required extension, name and other features of the application must be specified

        //application definition specifies names, version and required vulkan version of the program
        VkApplicationInfo appInfo{};
        appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
        appInfo.pApplicationName = "Assignment13";
        appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.pEngineName = "No Engine";
        appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0);
        appInfo.apiVersion = VK_API_VERSION_1_0;

        //This struct is not optional and tells the Vulkan driver which global extensions and validation layers we want to use
        VkInstanceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        createInfo.pApplicationInfo = &appInfo; //nested structure, link the above structure in CreateInfo one


        //The next two layers specify the desired global extensions
        uint32_t glfwExtensionCount = 0;
        const char** glfwExtensions;
        glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount); //this specify the needed extension of the OS
        createInfo.enabledExtensionCount = glfwExtensionCount;
        createInfo.ppEnabledExtensionNames = glfwExtensions;
        createInfo.enabledLayerCount = 0;

        //this create the isntance with the specified values
        if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
            throw std::runtime_error("failed to create instance!");
        }
    }


    //POINT 3 OF THE ASSIGNMENT - Create a Presentation Surface with GLFW
    void createSurface() {
        //the screen area where the host OS allow vulkan to draw images is called Presentation Surface
        //To work well vulkan need to acquire it from the OS: windows and istance has to be passed 
        VkWin32SurfaceCreateInfoKHR createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hwnd = glfwGetWin32Window(window);
        createInfo.hinstance = GetModuleHandle(nullptr);

        //the presentation surface requires bot the windows and the vulkan instance
        if (glfwCreateWindowSurface(instance, window, nullptr, &surface) != VK_SUCCESS) {
            throw std::runtime_error("failed to create window surface!");
        }
    }

    //2 - pysical device (GPU Selection)
    void pickPhysicalDevice() {
        
        //count  all the physical devices
        uint32_t deviceCount = 0;
        vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
        
        //check if there is a grafich card available
        if (deviceCount == 0) {
            throw std::runtime_error("failed to find GPUs with Vulkan support!");
        }

        //enumerate all physical devices
        std::vector<VkPhysicalDevice> devices(deviceCount);
        vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

        //pick the best physical devices
        for (const auto& device : devices) {
            if (isDeviceSuitable(device)) {
                physicalDevice = device;
                break;
            }
        }

        if (physicalDevice == VK_NULL_HANDLE) {
            throw std::runtime_error("failed to find a suitable GPU!");
        }

    }

    //check if the physical device has all the needed features
    bool isDeviceSuitable(VkPhysicalDevice device) {

        //this 2 functions are not needed here, just to know that they are present
        //get Basic device properties like the name, type and supported Vulkan version -USELESS FOR THE ASSIGNMENT
        VkPhysicalDeviceProperties deviceProperties;
        vkGetPhysicalDeviceProperties(device, &deviceProperties);

        //get information about The support for optional features like texture compression, 64 bit floats and multi viewport rendering - USELESS FOR THE ASSIGNMENT
        VkPhysicalDeviceFeatures deviceFeatures;
        vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

        //THIS COULD BE A POSSIBLE GPU SELECTION EXAMPLE BUT IN OUR ASSIGNMENT WE NEED DEVICE WITH OTHER FEATURES
        //return deviceProperties.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU &&
        //    deviceFeatures.geometryShader;
        //return true;

        //we can use it as a check to ensure that the device can process the command that we want (has graphic and presentation queue)
        QueueFamilyIndices indices = findQueueFamilies(device);
        return indices.isComplete();
    }

    //We need to check which queue families are supported by the device and which one of these supports the commands that we want to use
    QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device) {
        QueueFamilyIndices indices;

        //Queues are grouped into Families, each one supporting different type of operations they can execute
        //retrieving the list of queue families - retrive all information about memory types and heaps 
        //count them
        uint32_t queueFamilyCount = 0;
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);
        //retrive them
        std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
        vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

        int i = 0;
        //POINT 4 OF THE ASSIGNMENT
        // 4- cycle through the available device and pick up the first supporting both a graphic and a presentation queue
        for (const auto& queueFamily : queueFamilies) {

            if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
                indices.graphicsFamily = i;
            }
            //presentation support check
            VkBool32 presentSupport = false;
            vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

            if (presentSupport) {
                indices.presentFamily = i;
            }
            //A Computer Graphics Application, requires at least a Graphicand a Presentation queue
            if (indices.isComplete()) {
                break;
            }
            i++;
        }

        return indices;
    }

    //3 - Logical Device
    //Logical Devices allow to use different configurations of the same GPU for different purposes
    void createLogicalDevice() {
        //The creation of a logical device involves specifying a bunch of details - specifically we have to set all the queue
        QueueFamilyIndices indices = findQueueFamilies(physicalDevice);

        //create a queue info
        std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
        std::set<uint32_t> uniqueQueueFamilies = { indices.graphicsFamily.value(), indices.presentFamily.value() };

        float queuePriority = 1.0f;
        //for each requested queue both the family index and the count is interted in the structure
        for (uint32_t queueFamily : uniqueQueueFamilies) {
            VkDeviceQueueCreateInfo queueCreateInfo{};
            queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            queueCreateInfo.queueFamilyIndex = queueFamily;
            queueCreateInfo.queueCount = 1;
            queueCreateInfo.pQueuePriorities = &queuePriority; //here i can set queue priority
            queueCreateInfos.push_back(queueCreateInfo);
        }

        VkDeviceCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        //queue are created in the info of the device 
        createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size());
        createInfo.pQueueCreateInfos = queueCreateInfos.data();

        //The next information to specify is the set of device features that we'll be using. //no one in this assignment
        VkPhysicalDeviceFeatures deviceFeatures{};

        createInfo.pEnabledFeatures = &deviceFeatures;
        createInfo.enabledExtensionCount = 0;

        //POINT 5 OF THE ASSIGNMENT - Create a Logical Device with both a graphic queue and a presentation queue
        if (vkCreateDevice(physicalDevice, &createInfo, nullptr, &device) != VK_SUCCESS) {
            throw std::runtime_error("failed to create logical device!");
        }

        //a computer graphics Application requirea at least a Graphic and a Presentation Queue
        //retriving  queue handles 
        vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0, &graphicsQueue);
        vkGetDeviceQueue(device, indices.presentFamily.value(), 0, &presentQueue);
    }
    //POINT 6 OF ASSIGNMENT - Create a Command Pool
    void createCommandPool() {
        //since the use of several command buffers is common, they are allocated from a large grouos called CommandPools
        //each commandPools is strctly connected to the Queue Families 
        QueueFamilyIndices queueFamilyIndices = findQueueFamilies(physicalDevice);

        VkCommandPoolCreateInfo poolInfo{};
        poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
        poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
        //we are creating a command buffer for graphicsFamily queue
        poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value();

        if (vkCreateCommandPool(device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
            throw std::runtime_error("failed to create command pool!");
        }

    }
    //POINT 7 OF ASSIGNMENT - Create a Command Buffer from the previous Command Pool
    void createCommandBuffer() {

        VkCommandBufferAllocateInfo allocInfo{};
        allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
        allocInfo.commandPool = commandPool;
        allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
        allocInfo.commandBufferCount = 1; //command buffer 

        if (vkAllocateCommandBuffers(device, &allocInfo, &commandBuffer) != VK_SUCCESS) {
            throw std::runtime_error("failed to allocate command buffers!");
        }
    }

	void initApp() {
		// This will not be needed in this assignment!
	}

    void mainLoop() {
        std::cout << "ASSIGNMENT COMPLETE!";
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();
        }
    }

    //POINT 8 OF THE ASSIGNMENT - Release all the resources.
    void cleanup() {

    	// Add code to release all resources
        //destroy command pool 
        vkDestroyCommandPool(device, commandPool, nullptr);
        //destroy Presentation Surface
        vkDestroySurfaceKHR(instance, surface, nullptr);
        //destroy virtual device
        vkDestroyDevice(device, nullptr);
        //destroy Instance
        vkDestroyInstance(instance, nullptr);
        //destroy Windows
        glfwDestroyWindow(window);
        //the GLFW library requires also this call to freeing the it remaining resources
        glfwTerminate();
    }
};

int main() {
    Assignment13 app;

    try {
        app.run();
        
    } catch (const std::exception& e) {
        std::cerr << e.what() << std::endl;
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}