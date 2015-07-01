#!/bin/sh


echo "{
	\"m_type\":\"sum\",
	\"parcels\":[
		{
			\"m_type\":\"product\",
			\"parcels\":[
				{
					\"m_type\": \"const\",
					\"value\": $1
				},
				{
					\"m_type\":\"sum\",
					\"parcels\":[
						{
							\"m_type\": \"var\",
							\"index\": 0,
							\"coeff\": 1
						},
						{
							\"m_type\": \"var\",
							\"index\": 3,
							\"coeff\": -$4
						}
					]
				}
			]
		},
		{
			\"m_type\":\"product\",
			\"parcels\":[
				{
					\"m_type\": \"const\",
					\"value\": $2
				},
				{
					\"m_type\":\"sum\",
					\"parcels\":[
						{
							\"m_type\": \"var\",
							\"index\": 1,
							\"coeff\": 1
						},
						{
							\"m_type\": \"var\",
							\"index\": 3,
							\"coeff\": -$5
						}
					]
				}
			]
		},
		{
			\"m_type\":\"product\",
			\"parcels\":[
				{
					\"m_type\": \"const\",
					\"value\": $3
				},
				{
					\"m_type\":\"sum\",
					\"parcels\":[
						{
							\"m_type\": \"var\",
							\"index\": 2,
							\"coeff\": 1
						},
						{
							\"m_type\": \"var\",
							\"index\": 3,
							\"coeff\": -$6
						}
					]
				}
			]
		}
	]
}"
