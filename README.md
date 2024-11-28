# Aspect PHP Extension

Aspect is a PHP extension that provides a collection of utilities designed to enhance your development workflow. By introducing advanced features like attribute-based memoization, Aspect allows developers to write more efficient, clean, and maintainable code without the overhead of implementing complex patterns manually.

## Table of Contents

- [Installation](#installation)
- [Features](#features)
    - [Memoization with Attributes](#memoization-with-attributes)
        - [Usage](#usage)
        - [Examples](#examples)
        - [Benefits](#benefits)
- [License](#license)

## Installation

To install the Aspect extension, you can use [Pie](https://github.com/php/pie):

```bash
pie install solidworx/aspect
```

## Features

### Memoization with Attributes

The `#[Memoize]` attribute enables you to effortlessly cache the results of function or method calls based on their input arguments. This means that repeated calls with the same parameters will return the cached result instantly, without re-executing the underlying code.

#### Usage

To use the memoization feature, simply add the `#[Memoize]` attribute above the function or method you wish to cache.

```php
<?php

#[Memoize]
function expensiveComputation(int $x): int
{
    // Simulate a time-consuming operation
    sleep(2);
    return $x * $x;
}

$result = expensiveComputation(5); // Takes 2 seconds
$result = expensiveComputation(5); // Returns instantly from cache
```

The function `expensiveComputation` will execute only once for each unique argument value. Subsequent calls with the same argument will return the cached result immediately.

#### Examples

**Example 1: Memoizing a Function**

```php
<?php

#[Memoize]
function fibonacci(int $n): int
{
    if ($n <= 1) {
        return $n;
    }
    return fibonacci($n - 1) + fibonacci($n - 2);
}

echo fibonacci(10); // Outputs: 55
```

Without memoization, calculating `fibonacci(10)` would involve a significant number of redundant calculations. With the `#[Memoize]` attribute, each unique call is computed only once.

**Example 2: Memoizing a Class Method**

```php
<?php

class DataFetcher
{
    #[Memoize]
    public function fetchData(string $url): array
    {
        // Simulate an API call
        sleep(3);
        return ['data' => 'Sample data from ' . $url];
    }
}

$fetcher = new DataFetcher();
$data = $fetcher->fetchData('https://api.example.com'); // Takes 3 seconds
$data = $fetcher->fetchData('https://api.example.com'); // Returns instantly from cache
```

In this example, the `fetchData` method will only perform the API call once per unique URL.

**Example 3: Different Arguments Cache Separately**

```php
<?php

#[Memoize]
function multiply(int $a, int $b): int
{
    // Simulate a computation
    sleep(1);
    return $a * $b;
}

echo multiply(2, 3); // Takes 1 second
echo multiply(3, 2); // Takes 1 second (different arguments)
echo multiply(2, 3); // Returns instantly from cache
```

Each unique set of arguments is cached separately. The order and value of arguments affect the cache.

#### Benefits

- **Performance Improvement**: Significantly reduce execution time for functions with expensive or repetitive computations.
- **Easy Integration**: Apply memoization without altering the core logic of your code.
- **Automatic Argument Handling**: Caches results based on function arguments, ensuring accurate and efficient caching.
- **Resource Optimization**: Lower CPU and memory usage by avoiding unnecessary recalculations.

#### How It Works

- **Argument-Based Caching**: The memoization mechanism generates a unique key based on the function name and its arguments. This key is used to store and retrieve the cached result.
- **Cache Storage**: The cache is stored in memory during the script execution. For long-running processes or applications requiring persistent caching, additional configuration may be necessary.
- **Scope**: The cache is scoped to the function or method. Cached results do not interfere with other functions or methods.

#### Considerations

- **Side Effects**: Ensure that memoized functions are pure (i.e., their output depends only on their input and they have no side effects) for predictable results.
- **Memory Usage**: Be cautious when memoizing functions with a large number of unique argument combinations, as this can increase memory consumption.

## License

Aspect is open-sourced software licensed under the [MIT license](LICENSE).
