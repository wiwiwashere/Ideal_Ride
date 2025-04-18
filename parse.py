#not useful since the queue-time api wouldn't give us the lat and lon of the rides anyways

import json, csv

# 1. Load the JSON file
with open("land.json") as f:
    data = json.load(f)

# 2. Flatten out every ride
rides = []
for land in data["lands"]:
    for ride in land["rides"]:
        rides.append({
            "id":   ride["id"],
            "name": ride["name"],
        })

# 3. Write to CSV for the next step
with open("rides.csv", "w", newline="", encoding="utf-8") as fout:
    writer = csv.DictWriter(fout, fieldnames=["id","name"])
    writer.writeheader()
    writer.writerows(rides)

print(f"Wrote {len(rides)} rides to rides.csv")
