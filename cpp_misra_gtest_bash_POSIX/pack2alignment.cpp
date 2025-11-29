## 2. Alignment

Memory alignment is how the CPU expects data to be placed in memory. Misaligned access can **slow down** or even **crash** some CPUs (especially ARM, like Raspberry Pi).

```cpp
struct Aligned {
    char a;
    int b;
} __attribute__((aligned(8))); // GCC-specific
```

* `__attribute__((aligned(n)))` aligns a struct to `n` bytes.
* ARM CPUs on Raspberry Pi sometimes need alignment for **peripherals or DMA**.

---

