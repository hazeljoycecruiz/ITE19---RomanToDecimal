#include <stdio.h>  // Include the standard input/output library
#include <string.h>  // Include the string manipulation library
#include <ctype.h>  // Include the character handling library

// Function prototypes
int romanToDecimal(const char *roman);  // Function to convert Roman numeral to decimal
void decimalToWords(int num, char *words);  // Function to convert number to words
void processExpression(char *expression, FILE *outputFile);  // Function to process each expression
int stringLength(const char *str);  // Custom function to get string length
int isAlpha(char c);  // Custom function to check if a character is alphabetic
int isSpace(char c);  // Custom function to check if a character is a space
int isValidRoman(const char *roman);  // Function to validate Roman numerals
int isValidOperator(char c);  // Function to validate the operator (+, -, *, =)
void stringCopy(char *dest, const char *src);  // Function to copy one string to another
void stringConcat(char *dest, const char *src);  // Function to concatenate two strings

// Custom string length function
int stringLength(const char *str) {
    int len = 0;  // Initialize length to 0
    while (str[len] != '\0') len++;  // Iterate through string until null terminator
    return len;  // Return length
}

// Custom isAlpha function
int isAlpha(char c) {
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');  // Return 1 if character is alphabetic, else 0
}

// Custom isSpace function
int isSpace(char c) {
    return c == ' ' || c == '\t' || c == '\n';  // Return 1 if character is a space, tab, or newline
}

// Function to validate a Roman numeral string
int isValidRoman(const char *roman) {
    for (int i = 0; roman[i] != '\0'; i++) {  // Loop through each character in the string
        char c = roman[i];  // Get current character
        if (c != 'I' && c != 'V' && c != 'X' && c != 'L' &&
            c != 'C' && c != 'D' && c != 'M') {  // Check if character is a valid Roman numeral
            return 0;  // Return 0 if invalid character
        }
    }
    return 1;  // Return 1 if all characters are valid Roman numerals
}

// Function to validate the operator
int isValidOperator(char c) {
    return c == '+' || c == '-' || c == '*' || c == '=';  // Return 1 if operator is valid
}

// Custom string copy function
void stringCopy(char *dest, const char *src) {
    while (*src) {  // Iterate through source string
        *dest++ = *src++;  // Copy character from source to destination
    }
    *dest = '\0';  // Add null terminator to destination
}

// Custom string concatenation function
void stringConcat(char *dest, const char *src) {
    while (*dest) dest++;  // Move to the end of destination string
    while (*src) {  // Iterate through source string
        *dest++ = *src++;  // Append character from source to destination
    }
    *dest = '\0';  // Add null terminator to destination
}

int main() {
    FILE *inputFile = fopen("Input.txt", "r");  // Open input file for reading
    FILE *outputFile = fopen("Output.txt", "w");  // Open output file for writing

    if (inputFile == NULL || outputFile == NULL) {  // Check if files were opened successfully
        printf("Error: Unable to open file.\n");  // Print error if files failed to open
        return 1;  // Exit program with error
    }

    char line[256];  // Array to store each line from the input file
    char ch;  // Variable to hold each character read
    int index = 0;  // Index to keep track of position in line

    // Manual line-by-line file reading
    while ((ch = fgetc(inputFile)) != EOF) {  // Read each character until EOF
        if (ch == '\n') {  // If newline is encountered
            line[index] = '\0';  // Terminate the current line
            processExpression(line, outputFile);  // Process the expression in the line
            index = 0;  // Reset index for the next line
        } else {
            line[index++] = ch;  // Append character to the line
        }
    }

    if (index > 0) {  // Handle the last line if no newline at EOF
        line[index] = '\0';  // Terminate the last line
        processExpression(line, outputFile);  // Process the last line
    }

    fclose(inputFile);  // Close the input file
    fclose(outputFile);  // Close the output file

    printf("Processing complete. Check Output.txt for results.\n");  // Print completion message
    return 0;  // Exit program successfully
}

// Function to process a single expression
void processExpression(char *expression, FILE *outputFile) {
    char roman1[50], roman2[50], operator;  // Arrays for Roman numerals and operator
    int i = 0, j = 0;  // Indices for parsing the expression

    // Parse the first Roman numeral
    while (isAlpha(expression[i])) {  // Read characters until non-alphabetic character
        roman1[j++] = expression[i++];  // Copy character to roman1
    }
    roman1[j] = '\0';  // Null-terminate the first Roman numeral

    // Skip spaces and get the operator if any
    while (isSpace(expression[i])) i++;  // Skip spaces
    operator = expression[i++];  // Get the operator

    // Parse the second Roman numeral if operator exists
    char result[50] = "";  // Array to store the result as a string
    if (isValidOperator(operator)) {  // Check if operator is valid
        j = 0;
        while (isSpace(expression[i])) i++;  // Skip spaces
        while (isAlpha(expression[i])) {  // Read the second Roman numeral
            roman2[j++] = expression[i++];  // Copy character to roman2
        }
        roman2[j] = '\0';  // Null-terminate the second Roman numeral

        // Validate Roman numerals
        if (!isValidRoman(roman1) || !isValidRoman(roman2)) {  // Check if Roman numerals are valid
            fprintf(outputFile, "Invalid Roman numeral(s). Skipping line.\n");  // Print error message
            return;  // Skip processing this expression
        }

        // Convert Roman numerals to decimal
        int num1 = romanToDecimal(roman1);  // Convert the first Roman numeral
        int num2 = romanToDecimal(roman2);  // Convert the second Roman numeral
        int resultValue = 0;  // Variable to hold the result

        // Perform the operation
        switch (operator) {  // Perform the operation based on the operator
            case '+': resultValue = num1 + num2; break;  // Add the numbers
            case '-': resultValue = num1 - num2; break;  // Subtract the numbers
            case '*': resultValue = num1 * num2; break;  // Multiply the numbers
            case '=': resultValue = num1; break;  // Return the first number
            default: return;  // Should not reach here
        }

        // Convert result to words
        decimalToWords(resultValue, result);  // Convert the result to words
        fputs(result, outputFile);  // Write the result to the output file
        fputs("\n", outputFile);  // Add a newline to the output file
    } else {
        // If no operator, just convert the first Roman numeral
        if (!isValidRoman(roman1)) {  // Check if Roman numeral is valid
            fprintf(outputFile, "Invalid Roman numeral(s). Skipping line.\n");  // Print error message
            return;  // Skip processing this expression
        }
        int num = romanToDecimal(roman1);  // Convert the Roman numeral to decimal
        decimalToWords(num, result);  // Convert the decimal number to words
        fputs(result, outputFile);  // Write the result to the output file
        fputs("\n", outputFile);  // Add a newline to the output file
    }
}

// Function to convert Roman numeral to decimal
int romanToDecimal(const char *roman) {
    int value = 0, prev = 0;  // Initialize value and previous value

    for (int i = stringLength(roman) - 1; i >= 0; i--) {  // Iterate through the Roman numeral
        int current = 0;  // Variable to hold the current value

        // Switch case to assign values to Roman numeral characters
        switch (roman[i]) {
            case 'I': current = 1; break;
            case 'V': current = 5; break;
            case 'X': current = 10; break;
            case 'L': current = 50; break;
            case 'C': current = 100; break;
            case 'D': current = 500; break;
            case 'M': current = 1000; break;
            default: return -1;  // Return -1 if invalid Roman numeral
        }

        if (current < prev) {  // If current value is less than previous, subtract
            value -= current;
        } else {
            value += current;  // Otherwise, add
        }
        prev = current;  // Update previous value
    }

    return value;  // Return the final decimal value
}

// Function to convert number to words (simple version)
void decimalToWords(int num, char *words) {
    // For simplicity, assume num is between 1 and 9999
    char *unit[] = {"", "One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight", "Nine"};
    char *ten[] = {"", "", "Twenty", "Thirty", "Forty", "Fifty", "Sixty", "Seventy", "Eighty", "Ninety"};
    char *teen[] = {"Ten", "Eleven", "Twelve", "Thirteen", "Fourteen", "Fifteen", "Sixteen", "Seventeen", "Eighteen", "Nineteen"};

    if (num == 0) {
        stringCopy(words, "Zero");  // Special case for zero
        return;
    }

    int thousands = num / 1000;  // Get thousands place
    int hundreds = (num % 1000) / 100;  // Get hundreds place
    int remainder = num % 100;  // Get remainder after hundreds

    // Start building the words for the number
    char result[256] = "";

    if (thousands > 0) {
        stringConcat(result, unit[thousands]);  // Add thousands part
        stringConcat(result, " Thousand ");  // Add "Thousand"
    }

    if (hundreds > 0) {
        stringConcat(result, unit[hundreds]);  // Add hundreds part
        stringConcat(result, " Hundred ");  // Add "Hundred"
    }

    if (remainder > 0) {
        if (remainder < 10) {
            stringConcat(result, unit[remainder]);  // Add units part
        } else if (remainder < 20) {
            stringConcat(result, teen[remainder - 10]);  // Add teens part
        } else {
            int tensPart = remainder / 10;  // Get tens place
            int unitsPart = remainder % 10;  // Get units place
            stringConcat(result, ten[tensPart]);  // Add tens part
            if (unitsPart > 0) {
                stringConcat(result, "-");
                stringConcat(result, unit[unitsPart]);  // Add units part
            }
        }
    }

    stringCopy(words, result);  // Copy result to words
}
