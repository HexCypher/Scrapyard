--.009
-- GUI.lua

-- Debug function to handle errors
local function debug(message)
    SendChatMessage(message, "CHANNEL", nil, 6)
end

-- Error handling
local function safeCall(func)
    local status, err = pcall(func)
    if not status then
        debug("Error: " .. err)
    end
end

-- Create font strings for displaying information
local nameText = GUIFrame:CreateFontString(nil, "OVERLAY")
nameText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
nameText:SetPoint("TOP", GUIFrame, "TOP", 0, -10)

local classText = GUIFrame:CreateFontString(nil, "OVERLAY")
classText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
classText:SetPoint("TOP", nameText, "BOTTOM", 0, -5)

local creatureTypeText = GUIFrame:CreateFontString(nil, "OVERLAY")
creatureTypeText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
creatureTypeText:SetPoint("TOP", classText, "BOTTOM", 0, -5)

local creatureFamilyText = GUIFrame:CreateFontString(nil, "OVERLAY")
creatureFamilyText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
creatureFamilyText:SetPoint("TOP", creatureTypeText, "BOTTOM", 0, -5)

local resistancesText = GUIFrame:CreateFontString(nil, "OVERLAY")
resistancesText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
resistancesText:SetPoint("TOP", creatureFamilyText, "BOTTOM", 0, -5)

local statsText = GUIFrame:CreateFontString(nil, "OVERLAY")
statsText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
statsText:SetPoint("TOP", resistancesText, "BOTTOM", 0, -5)

local armorText = GUIFrame:CreateFontString(nil, "OVERLAY")
armorText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
armorText:SetPoint("TOP", statsText, "BOTTOM", 0, -5)

local defenseText = GUIFrame:CreateFontString(nil, "OVERLAY")
defenseText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
defenseText:SetPoint("TOP", armorText, "BOTTOM", 0, -5)

local meleeAttackText = GUIFrame:CreateFontString(nil, "OVERLAY")
meleeAttackText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
meleeAttackText:SetPoint("TOP", defenseText, "BOTTOM", 0, -5)

local attackPowerText = GUIFrame:CreateFontString(nil, "OVERLAY")
attackPowerText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
attackPowerText:SetPoint("TOP", meleeAttackText, "BOTTOM", 0, -5)

local damageText = GUIFrame:CreateFontString(nil, "OVERLAY")
damageText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
damageText:SetPoint("TOP", attackPowerText, "BOTTOM", 0, -5)

local attackSpeedText = GUIFrame:CreateFontString(nil, "OVERLAY")
attackSpeedText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
attackSpeedText:SetPoint("TOP", damageText, "BOTTOM", 0, -5)

local rangedAttackText = GUIFrame:CreateFontString(nil, "OVERLAY")
rangedAttackText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
rangedAttackText:SetPoint("TOP", attackSpeedText, "BOTTOM", 0, -5)

local rangedAttackPowerText = GUIFrame:CreateFontString(nil, "OVERLAY")
rangedAttackPowerText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
rangedAttackPowerText:SetPoint("TOP", rangedAttackText, "BOTTOM", 0, -5)

local rangedDamageText = GUIFrame:CreateFontString(nil, "OVERLAY")
rangedDamageText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
rangedDamageText:SetPoint("TOP", rangedAttackPowerText, "BOTTOM", 0, -5)

local rangedSpeedText = GUIFrame:CreateFontString(nil, "OVERLAY")
rangedSpeedText:SetFont("Fonts\\FRIZQT__.TTF", 12, "OUTLINE")
rangedSpeedText:SetPoint("TOP", rangedDamageText, "BOTTOM", 0, -5)
--.009
-- GUI.lua (continued)

-- Function to update the target's information
local function UpdateTargetInfo()
    if UnitExists("target") then
        local name = UnitName("target")
        local health = UnitHealth("target")
        local maxHealth = UnitHealthMax("target")
        local mana = UnitMana("target")
        local maxMana = UnitManaMax("target")
        local armor = UnitArmor("target")
        local minDamage, maxDamage = UnitDamage("target")
        local attackPower, attackPowerMod = UnitAttackPower("target")
        local mainSpeed, offSpeed = UnitAttackSpeed("target")
        local rangedAttack = UnitRangedAttack("target")
        local rangedAttackPower, rangedAttackPowerMod = UnitRangedAttackPower("target")
        local rangedSpeed, rangedMinDamage, rangedMaxDamage = UnitRangedDamage("target")
        local className = UnitClass("target")
        local creatureFamily = UnitCreatureFamily("target")
        local creatureType = UnitCreatureType("target")
        local baseDefense, defenseModifier = UnitDefense("target")
        local mainHandAttack, offHandAttack = UnitAttackBothHands("target")

        local resistances = {}
        for i = 1, 6 do
            local base, total, bonus, minus = UnitResistance("target", i)
            local resistanceName = ({[1] = "Holy", [2] = "Fire", [3] = "Nature", [4] = "Frost", [5] = "Shadow", [6] = "Arcane"})[i]
            resistances[i] = string.format("%s: %d (Base: %d, Bonus: %d, Minus: %d)", resistanceName, total, base, bonus, minus)
        end

        local stats = {}
        for i = 1, 5 do
            local stat, effectiveStat, posBuff, negBuff = UnitStat("target", i)
            local statName = ({[1] = "Strength", [2] = "Agility", [3] = "Stamina", [4] = "Intellect", [5] = "Spirit"})[i]
            stats[i] = string.format("%s: %d (Effective: %d, Pos Buff: %d, Neg Buff: %d)", statName, stat, effectiveStat, posBuff, negBuff)
        end

        nameText:SetText(name)
        classText:SetText("Class: " .. (className or "Unknown"))
        creatureTypeText:SetText("Type: " .. (creatureType or "N/A"))
        creatureFamilyText:SetText("Family: " .. (creatureFamily or "N/A"))
        resistancesText:SetText("Resistances:\n" .. table.concat(resistances, "\n"))
        statsText:SetText("Stats:\n" .. table.concat(stats, "\n"))
        armorText:SetText("Armor: " .. armor)
        defenseText:SetText("Defense: " .. baseDefense .. " (" .. defenseModifier .. ")")
        meleeAttackText:SetText("Melee Attack: Main Hand: " .. mainHandAttack .. " / Off Hand: " .. (offHandAttack or "N/A"))
        attackPowerText:SetText("Melee Power: " .. attackPower .. " (" .. attackPowerMod .. ")")
        damageText:SetText("Melee Damage: " .. string.format("%.3f", minDamage) .. " - " .. string.format("%.3f", maxDamage))
        attackSpeedText:SetText("Mainhand Speed: " .. string.format("%.3f", mainSpeed) .. " / Offhand Speed: " .. (offSpeed and string.format("%.3f", offSpeed) or "N/A"))
        rangedAttackText:SetText("Ranged Attack: " .. rangedAttack)
        rangedAttackPowerText:SetText("Ranged Power: " .. rangedAttackPower .. " (" .. rangedAttackPowerMod .. ")")
        rangedDamageText:SetText("Ranged Damage: " .. string.format("%.3f", rangedMinDamage) .. " - " .. string.format("%.3f", rangedMaxDamage))
        rangedSpeedText:SetText("Ranged Speed: " .. string.format("%.3f", rangedSpeed) .. "s")
    else
        nameText:SetText("No Target")
        classText:SetText("")
        creatureTypeText:SetText("")
        creatureFamilyText:SetText("")
        resistancesText:SetText("")
        statsText:SetText("")
        armorText:SetText("")
        defenseText:SetText("")
        meleeAttackText:SetText("")
        attackPowerText:SetText("")
        damageText:SetText("")
        attackSpeedText:SetText("")
        rangedAttackText:SetText("")
        rangedAttackPowerText:SetText("")
        rangedDamageText:SetText("")
        rangedSpeedText:SetText("")
    end
end

-- Set the OnUpdate script to update the target's information
GUIFrame:SetScript("OnUpdate", UpdateTargetInfo)

-- Example usage
safeCall(function()
    -- Your code here
end)
