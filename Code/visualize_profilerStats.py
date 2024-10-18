import pandas as pd
import plotly.graph_objects as go
from plotly.subplots import make_subplots

# Step 1: Read the CSV file from the Data folder
df = pd.read_csv('./Data/profilerStatsCSV.csv')  # Adjust the path as necessary
df.columns = df.columns.str.strip()  # Strip whitespace from column names

# Step 2: Create subplots
fig = make_subplots(rows=2, cols=1, 
                    subplot_titles=("Time Metrics Comparison", "Count by Section"),
                    vertical_spacing=0.1)

# Define the time metrics and colors
time_metrics = ['Total Time', 'Average Time', 'Min Time', 'Max Time']
colors_time = ['blue', 'orange', 'green', 'red']

# Add bars for time metrics
for metric, color in zip(time_metrics, colors_time):
    fig.add_trace(go.Bar(
        x=df['Section Name'],
        y=df[metric],
        name=metric,
        marker_color=color,
        showlegend=True
    ), row=1, col=1)

# Add bars for Count in the second subplot
fig.add_trace(go.Bar(
    x=df['Section Name'],
    y=df['Count'],
    name='Count',
    marker_color='purple',
    showlegend=True
), row=2, col=1)

# Step 3: Update layout for the subplots
fig.update_layout(
    title="Profiler Stats by Section",
    xaxis_title="Section Name",
    yaxis_title="Value",
    barmode='group',  # Group bars together in each subplot
    template='plotly_white'
)

# Update the x-axis for the second plot
fig.update_xaxes(title_text="Section Name", row=2, col=1)

# Step 4: Show the interactive plot
fig.show()
