SELECT DISTINCT 
    i.item_id,
    i.name,
    g.total as cost,
    string_agg(DISTINCT t.tag, ', ') as tags,
    i.description
FROM items i
JOIN item_maps m ON i.item_id = m.item_id
JOIN item_tags t ON i.item_id = t.item_id
JOIN item_gold g ON i.item_id = g.item_id
WHERE m.map_id = '11' 
    AND m.enabled = true
    AND t.tag IN ('AttackSpeed', 'Damage', 'CriticalStrike', 'ArmorPenetration')
GROUP BY i.item_id, i.name, g.total, i.description
ORDER BY g.total;
