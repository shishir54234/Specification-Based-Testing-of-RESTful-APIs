const fs = require("fs");
const prettier = require("prettier");

// Get input and output file paths from command-line arguments
const inputFile = process.argv[2];
const outputFile = process.argv[3];

// Read the input JavaScript file
fs.readFile(inputFile, "utf8", async (err, data) => {
    if (err) {
        console.error("Error reading file:", err);
        return;
    }

    try {
        // Format the code using Prettier
        const formattedCode = await prettier.format(data, { parser: "babel" });

        // Write the formatted code to the output file
        fs.writeFile(outputFile, formattedCode, (err) => {
            if (err) {
                console.error("Error writing file:", err);
            } else {
                console.log(`Beautified code saved to ${outputFile}`);
            }
        });
    } catch (formatError) {
        console.error("Error formatting code:", formatError);
    }
});
