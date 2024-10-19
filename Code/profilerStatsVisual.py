import pandas as pd
import plotly.graph_objects as go

# Step 1: Read the CSV file from the Data folder
df = pd.read_csv('./Data/profilerStatsCSV.csv')  # Adjust path as necessary
df.columns = df.columns.str.strip()  # Strip whitespace from column names

# Step 2: Group by 'Section Name' to calculate the relevant metrics
grouped_df = df.groupby('Section Name').agg(
    Total_Time=('Total Time', 'sum'),
    Average_Time=('Average Time', 'mean'),
    Max_Time=('Max Time', 'max'),
    Min_Time=('Min Time', 'min'),
    Count=('Count', 'sum')
).reset_index()

# Step 3: Create the bar chart
fig = go.Figure()

# Add bars for each metric
fig.add_trace(go.Bar(
    x=grouped_df['Section Name'],
    y=grouped_df['Total_Time'],
    name='Total Time (seconds)',
    visible=True,  # Make this visible by default
    marker_color='blue'
))

fig.add_trace(go.Bar(
    x=grouped_df['Section Name'],
    y=grouped_df['Average_Time'],
    name='Average Time (seconds)',
    visible=True,  # Make this visible by default
    marker_color='green'
))

fig.add_trace(go.Bar(
    x=grouped_df['Section Name'],
    y=grouped_df['Max_Time'],
    name='Max Time (seconds)',
    visible=True,  # Make this visible by default
    marker_color='orange'
))

fig.add_trace(go.Bar(
    x=grouped_df['Section Name'],
    y=grouped_df['Min_Time'],
    name='Min Time (seconds)',
    visible=True,  # Make this visible by default
    marker_color='red'
))

# Add a separate trace for Count
fig.add_trace(go.Bar(
    x=grouped_df['Section Name'],
    y=grouped_df['Count'],
    name='Count',
    visible=False,  # Hide by default; only show when selected
    marker_color='purple'
))

# Create dropdown buttons for filtering metrics
dropdown_buttons = [
    {
        'label': 'All Time Metrics',  # Default view
        'method': 'update',
        'args': [{'visible': [True, True, True, True, False]}]  # Show all time metrics, hide Count
    },
    {
        'label': 'Total Time',
        'method': 'update',
        'args': [{'visible': [True, False, False, False, False]}]  # Show Total Time only
    },
    {
        'label': 'Average Time',
        'method': 'update',
        'args': [{'visible': [False, True, False, False, False]}]  # Show Average Time only
    },
    {
        'label': 'Max Time',
        'method': 'update',
        'args': [{'visible': [False, False, True, False, False]}]  # Show Max Time only
    },
    {
        'label': 'Min Time',
        'method': 'update',
        'args': [{'visible': [False, False, False, True, False]}]  # Show Min Time only
    },
    {
        'label': 'Count',
        'method': 'update',
        'args': [{'visible': [False, False, False, False, True]}]  # Show Count only
    }
]

# Update layout with dropdown menu
fig.update_layout(
    title='Profiling Stats by Section',
    xaxis_title='Section Name',
    yaxis_title='Time (seconds) or Count',
    barmode='group',  # Group bars side by side
    showlegend=True,
    updatemenus=[
        {
            'buttons': dropdown_buttons,
            'direction': 'down',
            'showactive': True,
            'x': 0.9,  # Position further to the left
            'xanchor': 'left',
            'y': 1.1,  # Adjust this value to move the dropdown lower
            'yanchor': 'top'
        }
    ]
)

# Step 4: Show the interactive plot
fig.show()
