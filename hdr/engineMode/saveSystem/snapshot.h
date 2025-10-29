
#ifndef SNAPSHOT_STRUCT_H
# define SNAPSHOT_STRUCT_H

# include <stdlib.h>

// The Data in the snapShots is formatted like this:
// The data* only holds SystemObjects
// Each system object is formatted like this:
// [uint64_t  ], [uint32_t], [uint8_t ], [uint32_t    ]
// [unique_key], [hash    ], [saveable], [size_of_data]
// Each systemObjects data is then formatted like this:
// [uint32_t      ], [uint32_t         ], [size]
// [component_type], [size_of_component], [data]

struct SnapShot
{
	uint64_t hash;
	uint32_t size;
	void *data;
};

#endif
