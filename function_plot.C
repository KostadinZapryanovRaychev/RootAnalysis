#include <TF1.h>

void function_plot()
{
    // 1. Define a function "f2" with the formula "[0]*sin([1]*x)/x"
    // The range for x is from 0.0 to 10.0.
    // [0] and [1] are parameters we can change later.
    TF1 *f2 = new TF1("f2", "[0]*sin([1]*x)/x", 0., 10.);

    // 2. Set the value of parameter [0] to 1
    f2->SetParameter(0, 1);

    // 3. Set the value of parameter [1] to 1
    f2->SetParameter(1, 1);

    // 4. Draw the function on the canvas
    f2->Draw();
}

// The reason you see an empty plot is that the function object f2 is created on the stack,
// so it is automatically deleted as soon as the function_plot() function finishes executing.
// By the time the ROOT canvas tries to refresh or display it, the function no longer exists.

// To fix this, you need to create the function on the heap using the new keyword,
// so it stays in memory after the script finishes.

// Here is the fix: