typedef struct random_series
{
    f32_4x State;
} random_series;

typedef struct random_seed_params {
    uint32 E0;
    uint32 E1;
    uint32 E2;
    uint32 E3;
} random_seed_params;

inline random_seed_params
GetDefaultRandomSeeds()
{
    random_seed_params Result = {
        .E0 = 78953890,
        .E1 = 235498,
        .E2 = 893456,
        .E3 = 93453080,
    };
    return (Result);
}

inline random_series 
RandomSeed(random_seed_params Seeds)
{
    random_series Series;

    Series.State = U32_4x(Seeds.E0, Seeds.E1, Seeds.E2, Seeds.E3);

    return (Series);
}

inline random_series 
RandomSeedOffset(uint32 Offset)
{
    random_seed_params Params = {
        .E0 = Offset + 78953890,
        .E1 = Offset + 235498,
        .E2 = Offset + 893456,
        .E3 = Offset + 93453080,
    };

    random_series Series = RandomSeed(Params);

    return (Series);
}

inline f32_4x 
RandomNextU324X(random_series* Series)
{
    f32_4x Result = Series->State;
    f32_4x_OR_assign_f32_4x(&Result, ShiftLeft4X(Result, 13));
    f32_4x_OR_assign_f32_4x(&Result, ShiftLeft4X(Result, 17));
    f32_4x_OR_assign_f32_4x(&Result, ShiftLeft4X(Result, 5));
    Series->State = Result;

    return (Result);
}

inline u32 
RandomNextU32(random_series* Series)
{
    u32 Result = RandomNextU324X(Series).U32[0];
    return (Result);
}

inline uint32 RandomChoice(random_series* Series, uint32 ChoiceCount)
{
    uint32 Result = (RandomNextU32(Series) % ChoiceCount);

    return (Result);
}

inline real32 RandomUnilateral(random_series* Series)
{
    real32 Divisor = 1.0f / (real32)U32Max;
    real32 Result = Divisor * (real32)RandomNextU32(Series);

    return (Result);
}

inline real32 RandomBilateral(random_series* Series)
{
    real32 Result = 2.0f * RandomUnilateral(Series) - 1.0f;

    return (Result);
}

inline real32 
real32_RandomBetween(random_series* Series, real32 Min, real32 Max)
{
    real32 Result = Lerp(Min, RandomUnilateral(Series), Max);

    return (Result);
}

inline int32 
int32_RandomBetween(random_series* Series, int32 Min, int32 Max)
{
    int32 Result = Min + (int32)(RandomNextU32(Series) % ((Max + 1) - Min));

    return (Result);
}

inline f32_4x
RandomUnilateral_4x(random_series* Series)
{
    
    f32_4x Divisor = F32_4x(1.0f / (f32)S32Max);
    f32_4x Result = f32_4x_multiply_f32_4x(Divisor, (U32ToF32(f32_4x_AND_f32_4x(RandomNextU324X(Series), U32_4x_1xU32(S32Max)))));

    return (Result);
}

inline f32_4x
RandomBilateral_4x(random_series* Series)
{
    f32_4x Result = // F32_4x(2.0f) * RandomUnilateral_4x(Series) - F32_4x(1.0f);
        f32_4x_minus_f32_4x(f32_4x_multiply_f32_4x(F32_4x(2.0f), RandomUnilateral_4x(Series)), F32_4x(1.0f));
    return (Result);
}

typedef struct random_series_pcg
{
    u64 State;
    u64 Selector;
} random_series_pcg;

inline random_series_pcg
RandomSeedPCG(u64 State, u64 Selector)
{
    random_series_pcg Result;

    Result.State = State;
    Result.Selector = (Selector << 1) | 1;

    return (Result);
}

inline u32 
random_series_pcg_RandomNextU32(random_series_pcg* Series)
{
    u64 State = Series->State;
    State = State * 6364136223846793005ULL + Series->Selector;
    Series->State = State;

    u32 PreRotate = (u32)((State ^ (State >> 18)) >> 27);
    u32 Result = RotateRight(PreRotate, (s32)(State >> 59));

    return (Result);
}

inline real32 
random_series_pcg_RandomUnilateral(random_series_pcg* Series)
{
    real32 Divisor = 1.0f / (real32)U32Max;
    real32 Result = Divisor * (real32)random_series_pcg_RandomNextU32(Series);
    return (Result);
}

inline real32 
random_series_pcg_RandomBilateral(random_series_pcg* Series)
{
    real32 Result = 2.0f * random_series_pcg_RandomUnilateral(Series) - 1.0f;
    return (Result);
}
