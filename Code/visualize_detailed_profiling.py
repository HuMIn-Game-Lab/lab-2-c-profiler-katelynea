import pandas as pd
import plotly.graph_objects as go

# Step 1: Read the CSV file from the Data folder
df = pd.read_csv('./Data/detailedStats.csv')  # Adjust path as necessary
df.columns = df.columns.str.strip()  # Strip whitespace from column names
df['Time'] = pd.to_numeric(df['Time'], errors='coerce')  # Converts 'Time' to float

# Print the columns and data types for debugging
print(df.columns)  # Check column names
print(df.dtypes)   # Check data types

# Step 2: Create the line chart
fig = go.Figure()

# Add a line for each section's execution time
for section in df['Section Name'].unique():
    filtered_df = df[df['Section Name'] == section]
    
    # Debugging: Check how many records are there for each section
    print(f"{section}: {len(filtered_df)} records")  # Print number of records
    
    if not filtered_df.empty:  # Ensure there's data to plot
        fig.add_trace(go.Scatter(x=filtered_df.index, 
                                 y=filtered_df['Time'], 
                                 mode='lines+markers', 
                                 name=section))

# Step 3: Update layout (no dropdown needed)
fig.update_layout(
    title="Profiling Detailed Stats by Section (Line Chart)",
    xaxis_title="Execution Instances",
    yaxis_title="Time (seconds)",
    showlegend=True
)

# Step 4: Show the interactive plot
fig.show()
