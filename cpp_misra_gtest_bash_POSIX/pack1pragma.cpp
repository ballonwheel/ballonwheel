## 1. `#pragma` in Embedded C++

`#pragma` is a **compiler directive**. It gives instructions to the compiler that aren’t part of standard C++. Common uses in embedded programming:

### Example: Packing structures

```cpp
#include <iostream>

#pragma pack(push, 1) // Align to 1-byte boundary
struct PackedStruct {
    char a;
    int b;
};
#pragma pack(pop)

int main() {
    std::cout << "Size of packed struct: " << sizeof(PackedStruct) << std::endl;
}
```

**Explanation:**

* Normally, `sizeof(struct)` might be bigger because of padding for alignment.
* `#pragma pack(1)` removes padding, important in **embedded systems** where memory layout must match hardware registers.

---
